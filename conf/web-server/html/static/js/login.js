
/**添加设置cookie**/  
function addCookie(name,value,days,path)
{   

    var name = escape(name);  
    var value = escape(value);  

    var expires = new Date();  
    expires.setTime(expires.getTime() + days * 3600000 * 24);  
    //path=/，表示cookie能在整个网站下使用，path=/temp，表示cookie只能在temp目录下使用  
    path = path == "" ? "" : ";path=" + path;  
    //GMT(Greenwich Mean Time)是格林尼治平时，现在的标准时间，协调世界时是UTC  
    //参数days只能是数字型  
    var _expires = (typeof days) == "string" ? "" : ";expires=" + expires.toUTCString();  

    document.cookie = name + "=" + value + _expires + path;  
}  


/**获取cookie的值，根据cookie的键获取值**/  
function getCookieValue(name)
{  
    //用处理字符串的方式查找到key对应value  
    var name = escape(name);  
    //读cookie属性，这将返回文档的所有cookie  
    var allcookies = document.cookie;         
    //查找名为name的cookie的开始位置  
    name += "=";  
    var pos = allcookies.indexOf(name);      
    //如果找到了具有该名字的cookie，那么提取并使用它的值  
    if (pos != -1){                                             //如果pos值为-1则说明搜索"version="失败  
        var start = pos + name.length;                  //cookie值开始的位置  
        var end = allcookies.indexOf(";",start);        //从cookie值开始的位置起搜索第一个";"的位置,即cookie值结尾的位置  
        if (end == -1) end = allcookies.length;        //如果end值为-1说明cookie列表里只有一个cookie  
        var value = allcookies.substring(start,end); //提取cookie的值  
        return (value);                           //对它解码        
    }else{  //搜索失败，返回空字符串  
        return "";  
    }  
}  

/**根据cookie的键，删除cookie，其实就是设置其失效**/  
function deleteCookie(name,path)
{   
    var name = escape(name);  
    var expires = new Date(0);  
    path = path == "" ? "" : ";path=" + path;  
    document.cookie = name + "="+ ";expires=" + expires.toUTCString() + path;  
}  



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
