var x = 0;
var y = 0;
var width = 0;
var height = 0;
var untrimmedWidth = 0;
var untrimmedHeight = 0;

var setX = function(input)
{
    x = input;
	return;
};
setX.filterName = "setX";
Library.addFilter(setX.filterName);

var setY = function(input)
{
    y = input;
	return;
};
setY.filterName = "setY";
Library.addFilter(setY.filterName);

var setWidth = function(input)
{
    width = input;
	return;
};
setWidth.filterName = "setWidth";
Library.addFilter(setWidth.filterName);

var setHeight = function(input)
{
    height = input;
	return;
};
setHeight.filterName = "setHeight";
Library.addFilter(setHeight.filterName);

var setUntrimmedWidth = function(input)
{
    untrimmedWidth = input;
	return;
};
setUntrimmedWidth.filterName = "setUntrimmedWidth";
Library.addFilter(setUntrimmedWidth.filterName);

var setUntrimmedHeight = function(input)
{
    untrimmedHeight = input;
	return;
};
setUntrimmedHeight.filterName = "setUntrimmedHeight";
Library.addFilter(setUntrimmedHeight.filterName);

var checkPosition = function(input)
{
    return (x > 0 || y > 0 ? "true" : "");
};
checkPosition.filterName = "checkPosition";
Library.addFilter(checkPosition.filterName);

var checkSize = function(input)
{
	// must use weird arithmetic for comparison, comparing normal and untrimmed size always yields that they are different
    return (untrimmedWidth - width > 0 || untrimmedHeight - height > 0 ? "true" : "");
};
checkSize.filterName = "checkSize";
Library.addFilter(checkSize.filterName);

