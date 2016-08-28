/*
var addWxContact = function(wxid, cb)
{
    if (typeof WeixinJSBridge == 'undefined') {
        alert("no WeixinJSBridge");
        return false;
    }
    WeixinJSBridge.invoke('addContact', {
        webtype: '1',
        username: 'gh_c9f9f996603f'
    }, function(d) {
        // 返回d.err_msg取值，d还有一个属性是err_desc
        // add_contact:cancel 用户取消
        // add_contact:fail　关注失败
        // add_contact:ok 关注成功
        // add_contact:added 已经关注
        WeixinJSBridge.log(d.err_msg);
        cb && cb(d.err_msg);
    });
};
    */

function weixinShareTimeline(title, desc, link, imgUrl) {
    WeixinJSBridge.invoke('shareTimeline',{
        "img_url":imgUrl,
        //"img_width":"640",
        //"img_height":"640",
        "link":link,
        "desc": desc,
        "title":title
    });
}

function weixinSendAppMessage(title,desc,link,imgUrl) {
    WeixinJSBridge.invoke('sendAppMessage', {
        //"appid":appId,
        "img_url": imgUrl,
        //"img_width":"640",
        //"img_height":"640",
        "link": link,
        "desc": desc,
        "title": title
    });
}
