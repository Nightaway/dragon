function ActionResult(id, bag) {
    this.id = id;
    this.bag = bag;
}

function mergeJson(json1, json2) {
    for (x in json2) {
        json1[x] = json2[x];
    }
}

function ajs(request, response) {
    var conf = Library.Load('Config');

    var i = '1';
    var out = i + '2' + '3\n' +
                    request.protocol + '\n' +
                    request.method + '\n' +
                    request.ip + '\n' +
                    request.path + '\n' +
                    request.agent + '\n' +
                    request.lang + '\n' +
                    request.cookie['PCID'];

    response.status_code   = 200;
    response.content_type = 'text/html';
    response.content          = out;

    response.SetCookie('1', '2', new Date().toString(), '/', '');
}

function ad(request, response) {
    var sql = Library.Load('MySQL');
    var zoneid = parseInt(request.query['zoneid']);

    var banners = sql.queryAdsByZoneid(zoneid);

    if (banners.exCamps.length == 0) {
        return ;
    }

    var total_campaign_weight = 0;
    for (var i=0; i<banners.exCamps.length; ++i)
    {
        total_campaign_weight += banners.exCamps[i].weight;
    }

    for (var i=0; i<banners.exCamps.length; ++i)
    {
        banners.exCamps[i].weight /= total_campaign_weight;
    }

    var total  = 0;
    for (var i = 0; i < banners.exCamps.length; ++i) {
        var camps = banners.exCamps[i];
        for (var j = 0; j < camps.ads.length; ++j) {
            camps.ads[j].weight *= camps.weight;
            total += camps.ads[j].weight;
          }
    }

    for (var i = 0; i < banners.exCamps.length; ++i) {
        var camps = banners.exCamps[i];
        for (var j = 0; j < camps.ads.length; ++j) {
            camps.ads[j].weight /= total;
        }
    }

    var rnd   = Math.random();
    var low   = 0;
    var high  = 0;
    var selected;
    for (var i = 0; i < banners.exCamps.length; ++i) {
        var camps = banners.exCamps[i];
        for (var j = 0; j < camps.ads.length; ++j) {
            low = high;
            high += camps.ads[j].weight;
            if (rnd >= low && rnd <= high) {
                selected = camps.ads[j];
            }
        }
    }

    var conf = Compiler.LoadLibrary('Config');

    var out = JSON.stringify(banners);
    out += total_campaign_weight;

    var bag = conf.mysql;
    mergeJson(bag, conf.delivery);
    var rnd = { 'rnd': (parseInt(Math.random()*10000000).toString(16)) };
    mergeJson(bag, rnd);
    var result = new ActionResult(selected.id, bag)
    var instance = sql.queryInstanceByBannerid(selected.id);

    var out2 = JSON.stringify(selected);
    var out3 = JSON.stringify(result);
    response.status_code   = 200;
    response.content_type = 'text/html';
    response.content          = out2;

    result.instance = instance;
    return result;
}