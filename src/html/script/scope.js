var SlewRate = {
	Guide  : 0,
	Center : 1,
	Find   : 2,
	Max    : 3
};

var Direction = {
	North : 0,
	South : 1,
	East  : 2,
	West  : 3
};

function Scope ()
{
	this._ready = false;
	this._data = null;
	this._rate = SlewRate.Max;
	
	this.onReady = null;
}

Scope.prototype.formatDate = function (date)
{
	var ret = "";
	var d = date.getDate ();
	var m = date.getMonth () + 1;
	var y = date.getFullYear () % 100;
	
	if (m < 10)
		ret = ret + "0";
	ret = ret + m + "/";
	
	if (d < 10)
		ret = ret + "0";
	ret = ret + d + "/";
	
	if (y < 10)
		ret = ret + "0";
	ret = ret + y;
	
	return ret;
};

Scope.prototype.formatTime = function (date)
{
	var ret = "";
	var h = date.getHours ();
	var m = date.getMinutes ();
	var s = date.getSeconds ();

	if (h < 10)
		ret = ret + "0";
	ret = ret + h + ":";

	if (m < 10)
		ret = ret + "0";
	ret = ret + m + ":";

	if (s < 10)
		ret = ret + "0";
	ret = ret + s;

	return ret;
};

Scope.prototype.ready = function ()
{
	this._ready = true;
	
	this.send ("ECHO:0", true);
	while (!this._data)
		this._data = prompt ("PollMBED", "");
	this._data = null;

	var now = new Date ();

	var offset = now.getTimezoneOffset () / 60;
	var offstr = Math.abs (offset).toFixed (1);
	if (Math.abs (offset) < 10)
		offstr = "0" + offstr;
	if (offset < 0)
		offstr = "-" + offstr;
	else
		offstr = "+" + offstr;
	
	//this.command ("SG" + offstr, true);

	//this.command ("SC" + this.formatDate (now), true);
	//this.command ("SL" + this.formatTime (now), true);
	
	if (scope.onReady != null)
		scope.onReady ();
};

Scope.prototype.send = function (data)
{
	if (!this._ready)
		return false;
	
	var ret = prompt ("SendToMBED", data)
};

Scope.prototype.receive = function (b64data)
{
	this._data = Base64.decode (b64data);
};

Scope.prototype.command = function (cmd, resp)
{
	if (!this._ready)
		return false;

	if (resp)
	{
		this.send ("CMDR:" + cmd);
		
		while (!this._data)
			this._data = prompt ("PollMBED", "");
		
		var ret = this._data;
		this._data = null;
		return ret;
	}
	else
		this.send ("CMDN:" + cmd);
};

Scope.prototype.setSlewRate = function (rate)
{
	if (rate == SlewRate.Guide)
		this.command ("RG", false);
	else if (rate == SlewRate.Center)
		this.command ("RC", false);
	else if (rate == SlewRate.Find)
		this.command ("RM", false);
	else if (rate == SlewRate.Max)
		this.command ("RS", false);
	
	this._rate = rate;
};

Scope.prototype.slew = function (dir, rate)
{
	if (rate != this._rate)
		this.setSlewRate (rate);

	if (dir == Direction.North)
		this.command ("Mn");
	else if (dir == Direction.South)
		this.command ("Ms");
	else if (dir == Direction.East)
		this.command ("Me");
	else if (dir == Direction.West)
		this.command ("Mw");
};

Scope.prototype.halt = function ()
{
	this.command ("Q", false);
};

var scope = new Scope ();

