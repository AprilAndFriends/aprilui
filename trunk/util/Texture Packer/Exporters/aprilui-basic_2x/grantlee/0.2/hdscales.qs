// halves output coordinates
var HdScales = function(input)
{
	return String(input / 2);
};
HdScales.filterName = "hdscales";
HdScales.isSafe = false;
Library.addFilter("HdScales");
