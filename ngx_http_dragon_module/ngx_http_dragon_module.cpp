extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#ifdef CR
#undef CR
#include <pbsApplication.h>
#endif

pbsApplication pbs;

extern "C"
ngx_int_t init_cxxmvc_module(ngx_cycle_t *cycle)
{
	pbs.Start();
	return 0;
}

extern "C"
void uninit_cxxmvc_module(ngx_cycle_t *cycle)
{
	pbs.End();
}

/*
 *
 */
typedef struct {
	ngx_str_t path;
} ngx_http_cxxmvc_loc_conf_t;

/*
 *
 */
static void *ngx_http_cxxmvc_create_loc_conf(ngx_conf_t *cf);

/*
 *
 */
static char *ngx_http_cxxmvc_merge_loc_conf(ngx_conf_t *cf,
					    void *parent,
					    void *child);
/*
 *
 */
static char *ngx_http_cxxmvc_read_conf(ngx_conf_t *cf,
				       ngx_command_t *cmd,
				       void *conf);
/*
 *
 */
static ngx_int_t ngx_http_cxxmvc_handler(ngx_http_request_t *r);

static ngx_command_t ngx_http_cxxmvc_commands[] = {
	{
		ngx_string("set_app"),
		NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
		ngx_http_cxxmvc_read_conf,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_cxxmvc_loc_conf_t, path),
		NULL
	},
	ngx_null_command
};

static ngx_http_module_t ngx_http_cxxmvc_module_ctx = {
		NULL,
		NULL,

		NULL,
		NULL,

		NULL,
		NULL,

		ngx_http_cxxmvc_create_loc_conf,
		ngx_http_cxxmvc_merge_loc_conf
};

ngx_module_t ngx_http_dragon_module = {
		 NGX_MODULE_V1,
		 &ngx_http_cxxmvc_module_ctx,
		 ngx_http_cxxmvc_commands,
		 NGX_HTTP_MODULE,
		 NULL,
		 init_cxxmvc_module,
		 NULL,
		 NULL,
		 NULL,
		 NULL,
		 uninit_cxxmvc_module,
		 NGX_MODULE_V1_PADDING
};

static uint8_t* 
get_raw_http_body( ngx_http_request_t* r, size_t* body_len )
{
    ngx_chain_t* bufs = r->request_body->bufs;
    *body_len = 0;

    ngx_buf_t* buf = NULL;
    uint8_t* data_buf = NULL;
    size_t content_length = 0;

    if ( r->headers_in.content_length == NULL )
    {
        return NULL;
    }

    // malloc space for data_buf

    content_length = atoi( (char*)(r->headers_in.content_length->value.data) );

    data_buf = ( uint8_t* )ngx_palloc( r->pool , content_length + 1 );

    size_t buf_length = 0;

    while ( bufs )
    {
        buf = bufs->buf;

        bufs = bufs->next;

        buf_length = buf->last - buf->pos ;

        if( *body_len + buf_length > content_length )
        {

            memcpy( data_buf + *body_len, buf->pos, content_length - *body_len );
            *body_len = content_length ;

            break;
        }

        memcpy( data_buf + *body_len, buf->pos, buf->last - buf->pos );
        *body_len += buf->last - buf->pos;
    }

    if ( *body_len )
    {
        data_buf[*body_len] = '\0';
    }

    return data_buf;
}

static ngx_int_t
ngx_http_add_header(ngx_http_request_t *r, ngx_str_t *value, ngx_str_t *key)
{
    ngx_table_elt_t  *cc;

    cc = (ngx_table_elt_t *)ngx_list_push(&r->headers_out.headers);
    if (cc == NULL) {
        return NGX_ERROR;
    }

    cc->hash = 1;
    cc->key.data = key->data;
    cc->key.len  = key->len;
    cc->value = *value;

    return NGX_OK;
}

static ngx_int_t
ngx_http_add_cookie(ngx_http_request_t *r, ngx_str_t value)
{
    ngx_table_elt_t  *cc;

    cc = (ngx_table_elt_t *)ngx_list_push(&r->headers_out.headers);
    if (cc == NULL) {
        return NGX_ERROR;
    }

    cc->hash = 1;
    ngx_str_set(&cc->key, "Set-Cookie");
    cc->value = value;

    return NGX_OK;
}

static std::string ngx_http_get_cookie(ngx_http_request_t *r);
static std::string ngx_http_get_language(ngx_http_request_t *r);

static void p2s_urlquery_process_handler(ngx_http_request_t *r)
{
	using dragon::kHttp_Method_Get;
	using dragon::kHttp_Method_Post;
	using dragon::kResponseTypeRedirect;
	using dragon::Post;
	using dragon::HeaderList;
	using dragon::CookieList;
	using dragon::StringRef;
	using dragon::HttpRequest;
	using dragon::HttpResponse;
	using dragon::HeaderInList;

	ngx_buf_t *buf = NULL;
	ngx_chain_t out;

	size_t bodylen = 0;
  	uint8_t* contents = get_raw_http_body(r, &bodylen);

	unsigned uriLen = r->uri.len;
	if (r->args.len > 0) 
		uriLen = r->uri.len + r->args.len + 1;
	
	struct sockaddr_in *sin;
	ngx_addr_t          addr;
	addr.sockaddr = r->connection->sockaddr;
	addr.socklen  = r->connection->socklen;
	sin = (struct sockaddr_in *)addr.sockaddr;
	char *ip = inet_ntoa(sin->sin_addr);

	StringRef IP(ip, strlen(ip));
	StringRef routingUrl((const char *)r->uri.data, uriLen);
	StringRef userAgent;
	if (r->headers_in.user_agent)
		userAgent = StringRef((const char *)r->headers_in.user_agent->value.data, r->headers_in.user_agent->value.len);
	
	std::string userCookie = ngx_http_get_cookie(r);
	std::string lang       = ngx_http_get_language(r);

	HttpRequest  req;
	HttpResponse res;

	switch (r->method) {
 		case NGX_HTTP_POST:
		req.SetMethod(kHttp_Method_Post);
		Post post;
		post.data = contents;
		post.len  = bodylen;
		req.SetPost(post);
		break;

		case NGX_HTTP_GET:
		req.SetMethod(kHttp_Method_Get);
		break;
	}

	
	req.SetUrl(routingUrl);
	req.SetIp(IP);
	//req.SetHost(StringRef(host, strlen(host)));
	req.SetUserAgent(userAgent);
	req.SetUserCookie(StringRef(userCookie.c_str(), userCookie.length()));
	req.SetAcceptLanguage(StringRef(lang.c_str(), lang.length()));
  req.ParseCookie();

	HeaderInList & headers_in_ = req.GetHeaders();
	ngx_http_headers_in_t headers_in = r->headers_in;
	ngx_list_t headers_list = headers_in.headers;
	ngx_list_part_t *part = &(headers_list.part);
	while (part) {
		for (int i=0; i<headers_list.nalloc-1; i++) {
			ngx_table_elt_t  *str = (ngx_table_elt_t  *)part->elts;
			if (str[i].hash == 0)
				continue;
			if (str[i].key.len > 0 && str[i].value.len > 0)	{
				std::string key((const char *)str[i].key.data, str[i].key.len);
				std::string value((const char *)str[i].value.data, str[i].value.len);
				headers_in_[key] = value;
				printf("key: %s value:%s\n", str[i].key.data, str[i].value.data);
			}
		}
		
		part = part->next;
	}

	pbs.ResponseRequest(req, res);

	ngx_str_t k = ngx_string("X-Powered-By");
	ngx_str_t v = ngx_string("dragon/3.2");;
	ngx_http_add_header(r, &v, &k);

	if (res.GetResponseType() == kResponseTypeRedirect)
	{
		ngx_str_t k = ngx_string("location");
		ngx_str_t v = {res.GetContent().length(), (u_char *)res.GetContent().c_str()};
		ngx_http_add_header(r, &v, &k);
	}

	HeaderList headers = res.GetHeaders();
	if (headers.size() > 0) 
	{
		HeaderList::iterator iter;
		HeaderList::iterator end = headers.end();
		for (iter = headers.begin(); iter != end; ++iter)
		{		
			ngx_str_t k;
			ngx_str_t v; 

			k.data = (u_char *)ngx_pcalloc(r->pool, iter->first.length()+1);
			k.len  = iter->first.length();
			strcpy((char *)k.data, (const char *)iter->first.c_str());

			v.data = (u_char *)ngx_pcalloc(r->pool, iter->second.length()+1);
			v.len  = iter->second.length();
			strcpy((char *)v.data, (const char *)iter->second.c_str());

			ngx_http_add_header(r, &v, &k);
		}
	}

	CookieList cookies = res.GetCookies();
	if (cookies.size() > 0)
	{
		CookieList::iterator iter;
		CookieList::iterator end = cookies.end();
		for (iter = cookies.begin(); iter != end; ++iter)
		{
			ngx_str_t v; 

			v.data = (u_char *)ngx_pcalloc(r->pool, iter->length()+1);
			v.len  = iter->length();
			strcpy((char *)v.data, (const char *)iter->c_str());

			ngx_http_add_cookie(r, v);
		}
	}

	r->headers_out.status             = res.GetStatusCode();
  r->headers_out.content_type.len   = res.GetContentType().length();
	r->headers_out.content_type.data  = (u_char *)res.GetContentType().c_str();

	if (res.GetDetaRef().length() > 0)
		r->headers_out.content_length_n = res.GetDetaRef().length();
  else 
    r->headers_out.content_length_n = res.GetContent().length();

  ngx_http_send_header(r);

  if (r->headers_out.status == 204) {
		ngx_http_finalize_request(r, 0);
		return ;
	}

  buf = (ngx_buf_t *)ngx_pcalloc(r->pool, sizeof(ngx_buf_t));

  if (buf == NULL) {
    ngx_log_error(NGX_LOG_ERR,
                  r->connection->log,
                  0,
                  "Failed to allocate response buffer.");
    ngx_http_finalize_request(r, NGX_HTTP_INTERNAL_SERVER_ERROR);
    return ;
  }

	if (res.GetDetaRef().length() > 0) {
		buf->pos = (u_char *)res.GetDetaRef().data();
    buf->last = buf->pos + res.GetDetaRef().length();
	} else {
    buf->pos = (u_char *)res.GetContent().c_str();
    buf->last = buf->pos + res.GetContent().length();
	}
  buf->memory   = 1; /* content is in read-only memory */
  buf->last_buf = 1; /* there will be no more buffers in the request */

	out.buf    = buf;
	out.next   = NULL;

	ngx_http_output_filter(r, &out);
	ngx_http_finalize_request(r, 0);
}

static ngx_int_t ngx_http_cxxmvc_handler(ngx_http_request_t *r)
{
	 ngx_int_t rc = NGX_DONE;
    rc = ngx_http_read_client_request_body( r, p2s_urlquery_process_handler );
    if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
        return rc;
    }

  return NGX_DONE;
}

static char *ngx_http_cxxmvc_read_conf(ngx_conf_t *cf,
				       ngx_command_t *cmd,
				       void *conf)
{
	ngx_http_core_loc_conf_t *core_conf;

	core_conf = (ngx_http_core_loc_conf_t *)ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	core_conf->handler = ngx_http_cxxmvc_handler;
	ngx_conf_set_str_slot(cf, cmd, conf);

	return NGX_CONF_OK;
}

static void *ngx_http_cxxmvc_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_cxxmvc_loc_conf_t *conf;

	conf = (ngx_http_cxxmvc_loc_conf_t *)ngx_pcalloc(cf->pool, sizeof(ngx_http_cxxmvc_loc_conf_t));
	if (conf == NULL) {
		return NGX_CONF_ERROR;
	}

	conf->path.len 	= (size_t)NGX_CONF_UNSET_UINT;
	conf->path.data	= (u_char *)NGX_CONF_UNSET_PTR;

	return conf;
}

static char *ngx_http_cxxmvc_merge_loc_conf(ngx_conf_t *cf,
					      void *parent,
					      void *child)
{
	ngx_http_cxxmvc_loc_conf_t *prev  = (ngx_http_cxxmvc_loc_conf_t *)parent;
	ngx_http_cxxmvc_loc_conf_t *conf  = (ngx_http_cxxmvc_loc_conf_t *)child;

	ngx_conf_merge_str_value(conf->path, prev->path, "/usr/lib/");
	return NGX_CONF_OK;
}

static std::string ngx_http_get_cookie(ngx_http_request_t *r)
{
	ngx_table_elt_t **elt = NULL; 
	ngx_array_t cookies = r->headers_in.cookies;

	if (cookies.nelts > 0)
	{
		elt = (ngx_table_elt_t **)cookies.elts;
		return std::string((const char *)elt[0]->value.data, elt[0]->value.len);
	}

	return std::string();
}

static std::string ngx_http_get_language(ngx_http_request_t *r)
{
	if (r->headers_in.accept_language)
		return std::string((const char *) r->headers_in.accept_language->value.data, r->headers_in.accept_language->value.len);
	else
		return std::string();
}
