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

	for (i = 2; i <= 8; i++)
	{
		$("#rate_" + i).attr ("rate", i);
		$("#rate_" + i).click (function ()
		{
			slewrate = $(this).attr ("rate");
			$("#rate a").removeClass ("active");
			$(this).addClass ("active");
			return false;
		});
	}
});

function onScopeReady ()
{
	$("#gvp").html (scope.command ("GVP", true));
	$("#gvn").html (scope.command ("GVN", true));
	$("#gvd").html (scope.command ("GVD", true));
	$("#gvt").html (scope.command ("GVT", true));
}

