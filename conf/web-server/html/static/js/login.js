

function showDefaultText(str, inputNode) {
    if (inputNode.value == '') {
        inputNode.value = str;
    }
}

function hideDefaultText(str, inputNode) {
    if (inputNode.value == str) {
        inputNode.value = '';
    }
}

function showDefaultPwd(str, inputNode) {
    if (inputNode.value == '') {
        inputNode.type = 'text';
        inputNode.value = str;
    }
}

function hideDefaultPwd(str, inputNode) {
    if (inputNode.value == str) {
        inputNode.value = '';
        inputNode.type = 'password';
    }
}

var ISMOBILE = 1;

function checkPC() {
    //判断浏览器客户端
    var sUserAgent = navigator.userAgent.toLowerCase();
    var bIsIpad = sUserAgent.match(/ipad/i) == "ipad";
    var bIsIphoneOs = sUserAgent.match(/iphone os/i) == "iphone os";
    var bIsMidp = sUserAgent.match(/midp/i) == "midp";
    var bIsUc7 = sUserAgent.match(/rv:1.2.3.4/i) == "rv:1.2.3.4";
    var bIsUc = sUserAgent.match(/ucweb/i) == "ucweb";
    var bIsAndroid = sUserAgent.match(/android/i) == "android";
    var bIsCE = sUserAgent.match(/windows ce/i) == "windows ce";
    var bIsWM = sUserAgent.match(/windows mobile/i) == "windows mobile";

    if (bIsIpad || bIsIphoneOs || bIsMidp || bIsUc7 || bIsUc || bIsAndroid || bIsCE || bIsWM) {//如果是上述设备就会以手机域名打开
        ISMOBILE = 1;
    }else{//否则就是电脑域名打开
        ISMOBILE = 0;
    }
}

window.onload = function() {

    checkPC();

    if (ISMOBILE == 0) {
        Zepto(".top_menu").css("width", "33%");
        Zepto(".top_menu").css("margin-left", "33%");
        Zepto(".info_div").css("width", "33%");
        Zepto(".info_div").css("margin-left", "33%");
        Zepto(".login_info_div").css("width", "33%");
        Zepto(".login_info_div").css("margin-left", "33%");
    }
};
