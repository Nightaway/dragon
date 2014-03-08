var config = {
	'actionSeperator' : {
		'ad' : '&',
		'ck' : '&',
		'lg' : '__'
	}
};

function ad(QueryString) {
	info.log('Hello!');
	console.log(QueryString['index'] + '\n');
	console.log(JSON.stringify(request) + '\n');

	response.StringResult('Hello');
}

function lg(QueryString) {
	response.StringResult('lg');
}

function ck(QueryString) {
	response.StringResult('ck');
}
