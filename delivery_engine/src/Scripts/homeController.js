
function index(QueryString)
{
	response.StringResult(QueryString['raw_qs']);
}

function find(QueryString)
{
	response.StringResult(QueryString['id']);
}

function goto(QueryString)
{
	response.RedirectResult(QueryString['url']);
}

function hello(QueryString)
{
	response.StringResult('hello' + QueryString['name']);
}
