
var regExp = new RegExp(/\/?(\w+)?\/?(\w+)?\??(.*)?/);

function Init() {
	console.log("UrlRouting Module Init\n");
}

function Dispose() {
	console.log("UrlRouting Module Dispose\n");
}

function Route(Url) {
	var routingTable = {
		'ControllerName' : 'home',
		'ActionName'     : 'index',
		'QueryString'    : ''
	};

	//console.log("route url:" + Url + "\n");
	var result = regExp.exec(Url);

	//console.log(JSON.stringify(result) + '\n');

	routingTable.ControllerName = result[1];
	routingTable.ActionName     = result[2];
	routingTable.QueryString    = result[3];
	return routingTable;
}
