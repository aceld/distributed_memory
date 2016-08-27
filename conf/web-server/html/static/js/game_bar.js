/*
Zepto(document).on('ajaxBeforeSend',function(e,xhr,options){
    console.log('here is ajaxBeforSend');
});

Zepto(document).on('ajaxSuccess', function(e,xhr,options) {
   console.log('here is ajaxSUCCESS');
});
*/

var CLICKEVENT = 'tap';
var ISMOBILE = 1;

/*
function testAjax() {
    Zepto.ajax({type:'get',url:'/ajax', data:{name:'纤罗猫'}, success:function(data){
        console.log("ajax success:"+ data);
    }, error: function(xhr, type){
            alert('Ajax error!');
            console.log("xhr="+xhr);
            console.log("type="+type);
        }
    });
}
*/
/*
function loadGames() {
    Zepto.ajax({type:'get',url:'/data', data:{cmd:'allString'}, success:function(data){
        console.log("ajax success:"+ data);
        Zepto(document.body).append(data);

    }, error: function(xhr, type){
        alert('Ajax error!');
        console.log("xhr="+xhr);
        console.log("type="+type);
    }
    });
}
*/

/*
function loadGamesJson() {
    Zepto.ajax({type:'get',url:'/data', data:{cmd:'allJson'}, dataType:'json',success:function(data){
        jsonStr = JSON.stringify(data);
        console.log("ajax success:"+ jsonStr);
        Zepto(document.body).append(jsonStr);

    }, error: function(xhr, type){
        alert('Ajax error!');
        console.log("xhr="+xhr);
        console.log("type="+type);
    }
    });
}
*/

var alreadyLoadGameCnt=0;
var perLoadCnt = 8;
function loadGamesBar(fromId, cnt, kind) {
    Zepto.ajax({type:'get',url:'/data', data:{cmd: kind, fromId:fromId, count:cnt}, dataType:'json',success:function(data){
        var games = eval(data.games);
        for (var i = 0; i < games.length; i++) {
            addOneGameBar(games[i].id, games[i].title_m, games[i].url, games[i].picurl_m, games[i].pv, games[i].descrip, games[i].hot);
            alreadyLoadGameCnt++;
        }
        myScroll.refresh();
    }, error: function(xhr, type){
        //alert('Ajax error!');
        console.log("xhr="+xhr);
        console.log("type="+type);
    }
    });
}

function loadGamesBarBykind(fromId, cnt, kind, type) {
    Zepto.ajax({type:'get',url:'/data', data:{cmd: kind, fromId:fromId, count:cnt, kind:type}, dataType:'json',success:function(data){
        var games = eval(data.games);
        for (var i = 0; i < games.length; i++) {
            addOneGameBar(games[i].id, games[i].title_m, games[i].url, games[i].picurl_m, games[i].pv, games[i].descrip, games[i].hot);
            alreadyLoadGameCnt++;
        }
        myScroll.refresh();
    }, error: function(xhr, type){
        //alert('Ajax error!');
        console.log("xhr="+xhr);
        console.log("type="+type);
    }
    });
}

function clickGame(id,name,url) {
    Zepto.ajax({type:'get', url:'/data', data:{cmd: 'increase', fileId:id}, dataType:'json', success:function(data) {
        //console.log("post success!!!" + data.result);
    }, error: function(xhr,type){
        console.log("xhr="+xhr);
        console.log("type="+type);
    }});

    location.href=url;
}

//作为游戏图片的id索引值
var game_logo_id = 0;
function addOneGameBar(id, name, url, picurl, pv, desc, hot) {
    var liObj = Zepto("<li>");
    var barObj = Zepto("<div>").attr("class", "game_opt");
    var aObj = Zepto("<a>").html("下载文件");
    //var canvasObj = Zepto("<canvas>").attr("id", "game_logo_" + game_logo_id).attr("width", 60).attr("height", 60);
    var gameLogoObj = Zepto("<img>").attr("id", "game_logo_" + game_logo_id).attr("width", 60).attr("height", 60).attr("class", "game_logo_img");
    var infoObj = Zepto("<div>").attr("class", "game_info");

    var gameNameObj = Zepto("<span>").attr("class", "game_name").html(name);

    var hotObj = Zepto("<div>").attr("class", "game_hot");
    var starObj = Zepto("<span>").attr("class", "star_icon");
    var pvObj = Zepto("<i>").html("下载次数: "+pv);

    var descObj = Zepto("<p>").html(desc);


    if (hot == 1) {
        var titleObj = Zepto("<span>").attr("class", "img_title_new");
        barObj.append(titleObj);
        titleObj.html("火爆");
    }



    hotObj.append(starObj);
    hotObj.append(pvObj);

    infoObj.append(gameNameObj);
    infoObj.append(hotObj);
    infoObj.append(descObj);

    //barObj.append(canvasObj);
    barObj.append(gameLogoObj);
    barObj.append(infoObj);
    barObj.append(aObj);
    liObj.append(barObj).appendTo("#thelist");
    gameLogoObj.attr("src", picurl);
    //createCanvasFromUrl(game_logo_id,picurl, 0, 0);
/*    preImage(picurl, function() {
        gameLogoObj.attr("src", picurl);
    });*/
    game_logo_id++;


    /*
    if (ISMOBILE == 1) {
        barObj.on(CLICKEVENT, function(e){
            clickGame(id, name, url);
        });
    }
    */
    aObj.on(CLICKEVENT, function(e) {
        clickGame(id, name, url);
    });
    aObj.on(CLICKEVENT, function(e){
        aObj.css("color", "#ffffff");
        aObj.css("background-color", "#0c4bba");
        aObj.css("border-color", "#0c4bba");
    });
    aObj.on('longTap', function(e){
        aObj.css("color", "#ffffff");
        aObj.css("background-color", "#0c4bba");
        aObj.css("border-color", "#0c4bba");
    });
}

function createCanvasFromUrl(id, picurl, x, y) {
    var canvas = document.getElementById("game_logo_"+id);

    if (canvas.getContext){
        var context = canvas.getContext('2d');
        preImage(picurl,function(){
            context.drawImage(this,x,y,60,60);
        });
    }
}

//由于image加载是异步的，应该在image加载完毕时，调用drawImage方法绘制图像；可利用方法：
function preImage(url,callback){
    var img = new Image(); //创建一个Image对象，实现图片的预下载
    img.src = url;

    if (img.complete) { // 如果图片已经存在于浏览器缓存，直接调用回调函数
        callback.call(img);
        return; // 直接返回，不用再处理onload事件
    }

    img.onload = function () { //图片下载完毕时异步调用callback函数。
        callback.call(img);//将回调函数的this替换为Image对象
    };
}



/*
 *  iscroll  function
 *
 *
  */


var myScroll,
    pullDownEl, pullDownOffset,
    pullUpEl, pullUpOffset,
    generatedCount = 0;

function pullDownAction () {

    setTimeout(function () {	// <-- Simulate network congestion, remove setTimeout from production!
        /*
        var el, li, i;
        el = document.getElementById('thelist');

        for (i=0; i<3; i++) {
            li = document.createElement('li');
            li.innerText = 'Generated row ' + (++generatedCount);
            el.insertBefore(li, el.childNodes[0]);
        }
        */
        myScroll.refresh();		// Remember to refresh when contents are loaded (ie: on ajax completion)
    }, 500);	// <-- Simulate network congestion, remove setTimeout from production!

}

function pullUpAction () {
    setTimeout(function () {	// <-- Simulate network congestion, remove setTimeout from production!

        /*
        var el, li, i;
        el = document.getElementById('thelist');

        for (i=0; i<3; i++) {
            li = document.createElement('li');
            li.innerText = 'Generated row ' + (++generatedCount);
            el.appendChild(li, el.childNodes[0]);
        }
        */
        if (menu_game_new_selected == 1) {
            loadGamesBar(alreadyLoadGameCnt,perLoadCnt,'newFile');
        } else if (menu_game_hot_selected == 1) {
            loadGamesBar(alreadyLoadGameCnt,perLoadCnt,'hotGame');
        } else if (menu_kind_selected == 1 || menu_kind_selected_type != '0') {
            loadGamesBarBykind(alreadyLoadGameCnt,perLoadCnt,'kind', menu_kind_selected_type);
        }

    }, 1000);	// <-- Simulate network congestion, remove setTimeout from production!
}

function loaded() {

    pullDownEl = document.getElementById('pullDown');
    pullDownOffset = pullDownEl.offsetHeight;
    pullUpEl = document.getElementById('pullUp');
    pullUpOffset = pullUpEl.offsetHeight;

    myScroll = new iScroll('wrapper', {
        useTransition: true,
        topOffset: pullDownOffset,
        onRefresh: function () {
            if (pullDownEl.className.match('loading')) {
                pullDownEl.className = '';
                //pullDownEl.querySelector('.pullDownLabel').innerHTML = 'Pull down to refresh...';
            } else if (pullUpEl.className.match('loading')) {
                pullUpEl.className = '';
                pullUpEl.querySelector('.pullUpLabel').innerHTML = '轻轻上拉 更多文件...';
            }
        },
        onScrollMove: function () {
            hideMenu();
            if (this.y > 5 && !pullDownEl.className.match('flip')) {
                pullDownEl.className = 'flip';
                //pullDownEl.querySelector('.pullDownLabel').innerHTML = 'Release to refresh...';
                this.minScrollY = 0;
            } else if (this.y < 5 && pullDownEl.className.match('flip')) {
                pullDownEl.className = '';
                //pullDownEl.querySelector('.pullDownLabel').innerHTML = 'Pull down to refresh...';
                this.minScrollY = -pullDownOffset;
            } else if (this.y < (this.maxScrollY - 5) && !pullUpEl.className.match('flip')) {
                pullUpEl.className = 'flip';
                pullUpEl.querySelector('.pullUpLabel').innerHTML = '刷新中...';
                this.maxScrollY = this.maxScrollY;
            } else if (this.y > (this.maxScrollY + 5) && pullUpEl.className.match('flip')) {
                pullUpEl.className = '';
                pullUpEl.querySelector('.pullUpLabel').innerHTML = '轻轻上拉 更多文件...';
                this.maxScrollY = pullUpOffset;
            }
        },
        onScrollEnd: function () {
            if (pullDownEl.className.match('flip')) {
                pullDownEl.className = 'loading';
                //pullDownEl.querySelector('.pullDownLabel').innerHTML = '上面没有游戏哦...';
                pullDownAction();	// Execute custom function (ajax call?)
            } else if (pullUpEl.className.match('flip')) {
                pullUpEl.className = 'loading';
                pullUpEl.querySelector('.pullUpLabel').innerHTML = '更新中...';
                pullUpAction();	// Execute custom function (ajax call?)
            }
        }
    });

    setTimeout(function () { document.getElementById('wrapper').style.left = '0'; }, 800);
    selected_action('newFile');
}

function hideMenu() {
    Zepto(".second_menu_show").attr("class", "second_menu_hide");
    menu_kind_selected = 0;
}

document.addEventListener('touchmove', function (e) { e.preventDefault(); }, false);

document.addEventListener('DOMContentLoaded', function () { setTimeout(loaded, 200); }, false);

var menu_game_new_selected = 0;
var menu_game_hot_selected = 0;
var menu_kind_selected = 0;
var menu_group_selected = 0;
var menu_kind_selected_type = '0';

function selected_action(kind) {

    if (kind == 'newFile') {
        //未被点中
        if (menu_game_new_selected == 0) {
            menu_game_new_selected = 1;
            menu_game_hot_selected = 0;
            menu_kind_selected = 0;

            //设置其他两个按钮未选中
            Zepto(".hot_icon img").attr("src", "static/img/icon_2@2x.png");
            Zepto(".kind_icon img").attr("src", "static/img/icon_3@2x.png");

            //使本按钮选中
            Zepto(".new_icon img").attr("src", "static/img/icon_11@2x.png")

            reloadGames('newFile');

        }
        //已经点中
        else {
            hideMenu();
            return;
        }
    } else if( kind == 'hotGame') {
        if (menu_game_hot_selected == 0) {
            menu_game_hot_selected = 1;
            menu_game_new_selected = 0;
            menu_kind_selected = 0;

            //设置其他两个按钮未选中
            Zepto(".new_icon img").attr("src", "static/img/icon_1@2x.png");
            Zepto(".kind_icon img").attr("src", "static/img/icon_3@2x.png");
            Zepto(".hot_icon img").attr("src", "static/img/icon_21@2x.png");

            //reloadGames('hotGame');
            location.href = "/upload.html";

        }
        else {
            return;
        }


    } else if (kind == 'group') {
        if (menu_group_selected == 0) {
            menu_group_selected = 1;
            menu_game_new_selected = 0;
            menu_kind_selected = 0;
            menu_game_hot_selected = 0;

            //设置其他三个按钮未选中
            Zepto(".new_icon img").attr("src", "static/img/icon_1@2x.png");
            Zepto(".kind_icon img").attr("src", "static/img/icon_3@2x.png");
            Zepto(".hot_icon img").attr("src", "static/img/icon_2@2x.png");
            Zepto(".group_icon img").attr("src", "static/img/icon_41@2x.png");

            //location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wx9324b266aa4818d0&redirect_uri=http%3A%2F%2Fm.wsq.qq.com%2F264281398&response_type=code&scope=snsapi_base&state=1#wechat_redirect'";
            location.href = "/login.html";
        }


    } else if (kind == 'kind') {
        if (menu_kind_selected == 0) {
            menu_kind_selected = 1;
            menu_game_new_selected=0;
            menu_game_hot_selected=0;
            //设置其他两个按钮未选中
            Zepto(".new_icon img").attr("src", "static/img/icon_1@2x.png");
            Zepto(".hot_icon img").attr("src", "static/img/icon_2@2x.png");
            Zepto(".kind_icon img").attr("src", "static/img/icon_31@2x.png");


            Zepto("#second_menu").each(function(index){
                Zepto(this).attr('class','second_menu_show');
            });

            Zepto("#menu_kind_0 i").html("("+gameCntAll+")");
            Zepto("#menu_kind_1 i").html("("+gameCnt_1+")");
            Zepto("#menu_kind_2 i").html("("+gameCnt_2+")");
            Zepto("#menu_kind_3 i").html("("+gameCnt_3+")");
            Zepto("#menu_kind_4 i").html("("+gameCnt_4+")");
            Zepto("#menu_kind_5 i").html("("+gameCnt_5+")");
            Zepto("#menu_kind_6 i").html("("+gameCnt_6+")");


            return;
        }
        else {
            menu_kind_selected = 0;
            hideMenu();
        }
    } else {
        menu_kind_selected_type = kind.split('_')[1];

        reloadGamesKind('kind', menu_kind_selected_type);
    }

}



function reloadGames(kind) {
    //隐藏二级菜单
    hideMenu();
    //清楚iScroll元素
    Zepto("#thelist li:not(:first-child)").remove();
    //游戏计数清零
    alreadyLoadGameCnt = 0;
    //重新加载一类游戏
    loadGamesBar(alreadyLoadGameCnt, perLoadCnt, kind);


    myScroll.refresh();
}
function reloadGamesKind(kind,type) {
    //隐藏二级菜单
    hideMenu();
    //清楚iScroll元素
    Zepto("#thelist li:not(:first-child)").remove();
    //游戏计数清零
    alreadyLoadGameCnt = 0;
    //重新加载一类游戏
    loadGamesBarBykind(alreadyLoadGameCnt, perLoadCnt, kind, type);


    myScroll.refresh();
}


//所有游戏个数
var gameCntAll = 0;
//体育竞技游戏个数
var gameCnt_1 = 0;
//休闲益智游戏个数
var gameCnt_2 = 0;
//角色扮演游戏个数
var gameCnt_3 = 0;
//飞行射击游戏个数
var gameCnt_4 = 0;
//棋牌娱乐游戏个数
var gameCnt_5 = 0;
//女孩专题游戏个数
var gameCnt_6 = 0;

function selecteGameCnt() {
    Zepto.ajax({type:'get',url:'/data', data:{cmd: 'selectCnt', kind: 'all'}, dataType:'json',success:function(data){
        var allData = eval(data);
        gameCntAll = allData['kind_0'];
        gameCnt_1 = allData['kind_1'];
        gameCnt_2 = allData['kind_2'];
        gameCnt_3 = allData['kind_3'];
        gameCnt_4 = allData['kind_4'];
        gameCnt_5 = allData['kind_5'];
        gameCnt_6 = allData['kind_6'];
    }, error: function(xhr, type){
        //alert('Ajax error!');
        console.log("xhr="+xhr);
        console.log("type="+type);
    }});
}



var layerIsShow = 0;
function showOutLayer() {
    if (layerIsShow == 0) {
        Zepto("#out_layer").show();
        Zepto("#point").show();
        layerIsShow = 1;
    }
    else {
        Zepto("#out_layer").hide();
        Zepto("#point").hide();
        layerIsShow = 0;
    }
}

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
        CLICKEVENT = 'tap';
    }else{//否则就是电脑域名打开
        ISMOBILE = 0;
        CLICKEVENT = 'click';
    }
}

function bindEvent() {
    Zepto("#menu_game_new").on(CLICKEVENT, function(e) {
        selected_action('newFile');
    });

    Zepto("#menu_game_hot").on(CLICKEVENT, function(e) {
        selected_action('hotGame');
    });

    Zepto("#menu_group").on(CLICKEVENT, function(e){
        selected_action('group');
    });

    Zepto("#menu_kind").on(CLICKEVENT, function(e) {
        selected_action('kind');
    });

    Zepto("#menu_kind_0").on(CLICKEVENT, function(e){
        selected_action('newFile');
    });
    Zepto("#menu_kind_1").on(CLICKEVENT, function(e){
        selected_action('kind_1');
    });
    Zepto("#menu_kind_2").on(CLICKEVENT, function(e){
        selected_action('kind_2');
    });
    Zepto("#menu_kind_3").on(CLICKEVENT, function(e){
        selected_action('kind_3');
    });
    Zepto("#menu_kind_4").on(CLICKEVENT, function(e){
        selected_action('kind_4');
    });
    Zepto("#menu_kind_5").on(CLICKEVENT, function(e){
        selected_action('kind_5');
    });
    Zepto("#menu_kind_6").on(CLICKEVENT, function(e){
        selected_action('kind_6');
    });

    Zepto("#attention").on(CLICKEVENT, function(e) {
        location.href="http://mp.weixin.qq.com/s?__biz=MzA5MjA2NTY5MA==&mid=203311487&idx=1&sn=3fd67d411e686f7dc4cf1db9e6af2949#rd";
    });

    Zepto('#out_layer').on(CLICKEVENT, function(e){
        showOutLayer();
    });

    Zepto('#attention_please').on(CLICKEVENT, function(e){
        showOutLayer();
    });
}

window.onload = function() {
    //统计各种类文件个数
    //selecteGameCnt();

    checkPC();

    bindEvent();


    if (ISMOBILE == 0) {
        Zepto("#wrapper").css("width", "33%");
        Zepto("#wrapper").css("margin-left", "33%");
        Zepto("#header").css("width", "33%");
        Zepto("#header").css("margin-left", "33%");
        Zepto("#footer").css("width", "33%");
        Zepto("#footer").css("margin-left", "33%");

        Zepto("#attention").hide();
        Zepto("#attention_please").hide();
        Zepto("#weixin_qrcode").show();
        Zepto("#qrcode_desc").show();
    }
};

