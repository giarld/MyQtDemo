var SCALE = 720;

function dpi(win, size) {
    var dpx = Math.min(win.width, win.height);
    if(dpx <= 0)
        return size;
    var dp = (dpx * size) / SCALE;
    return dp;
}

function resetScale(scale) {
    SCALE = scale
}
