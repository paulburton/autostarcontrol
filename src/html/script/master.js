var slewrate = SlewRate.Max;

$(function ()
{
	scope.onReady = onScopeReady;

	$("#dpad_n").click (function ()
	{
		scope.slew (Direction.North, slewrate);
		return false;
	});
	$("#dpad_s").click (function ()
	{
		scope.slew (Direction.South, slewrate);
		return false;
	});
	$("#dpad_e").click (function ()
	{
		scope.slew (Direction.East, slewrate);
		return false;
	});
	$("#dpad_w").click (function ()
	{
		scope.slew (Direction.West, slewrate);
		return false;
	});
	$("#dpad_h").click (function ()
	{
		scope.halt ();
		return false;
	});

	$("#rate_max").click (function ()
	{
		slewrate = SlewRate.Max;
		$("#rate a").removeClass ("active");
		$("#rate_max").addClass ("active");
		return false;
	});
	$("#rate_find").click (function ()
	{
		slewrate = SlewRate.Find;
		$("#rate a").removeClass ("active");
		$("#rate_find").addClass ("active");
		return false;
	});
	$("#rate_center").click (function ()
	{
		slewrate = SlewRate.Center;
		$("#rate a").removeClass ("active");
		$("#rate_center").addClass ("active");
		return false;
	});
	$("#rate_guide").click (function ()
	{
		slewrate = SlewRate.Guide;
		$("#rate a").removeClass ("active");
		$("#rate_guide").addClass ("active");
		return false;
	});
});

function onScopeReady ()
{
	$("#gvp").html (scope.command ("GVP", true));
	$("#gvn").html (scope.command ("GVN", true));
	$("#gvd").html (scope.command ("GVD", true));
	$("#gvt").html (scope.command ("GVT", true));
}

