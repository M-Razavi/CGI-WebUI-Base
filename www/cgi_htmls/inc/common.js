/* This script and many more are available free online at
   The JavaScript Source!! http://javascript.internet.com
   Created by: Konstantin Jagello | http://javascript-array.com/ */

function isMSIE() {
  if (window.navigator.appName == "Microsoft Internet Explorer") {
    if (window.navigator.appVersion.match(/Opera/)) {
      return false;
    }
    return true;
  }
  return false;
}

function isMSIE6() {
  if (isMSIE()) {
    if (window.navigator.appVersion.match(/compatible; MSIE ([^;]+);/)) {
      var ver = RegExp.$1
      /* 5.5�ȏ�A7.0������IE6.0�������� */
      if (ver >= 5.5 && ver < 7.0) {
        return true;
      }
    }
  }
  return false;
}

function isMSIE8() {
  if (isMSIE()) {
    if (window.navigator.appVersion.match(/compatible; MSIE ([^;]+);/)) {
      var ver = RegExp.$1
      /* 8.0�ȏ��IE8�������� */
      if (ver >= 8.0) {
        return true;
      }
    }
  }
  return false;
}

new function() {
  if (!isMSIE() || isMSIE8()) {
    /* IE6,7�ȊO�ł�menu�̍�����float�v�f������p���Ȃ��̂�JavaScript�őΏ� */
    var menu_adjust = function() {
      var menu = document.getElementById("menu");
      var mmenu1 = document.getElementById("mmenu1");
      if (menu && mmenu1 && mmenu1.offsetHeight) {
        var height = mmenu1.offsetHeight;
        menu.style.height = height + "px";
      }
    };
    if (isMSIE8())
      attachEvent("onload", menu_adjust);
    else
      addEventListener("load", menu_adjust, false);
  }
  if (isMSIE6()) {
    /* IE6�ł�min-width�����p�o���Ȃ��̂�JavaScript�őΏ� */
    var min_width = function() {
      var width = "auto";
      if (document.body.clientWidth < 800) {
        width = "780px";
      }
      var header = document.getElementById("header");
      if (header) {
        header.style.width = width;
      }
      var title = document.getElementById("title");
      if (title) {
        title.style.width = width;
      }
      var menu = document.getElementById("menu");
      if (menu) {
        menu.style.width = width;
      }
    };
    attachEvent("onload", min_width);
    attachEvent("onresize", min_width);
  }
}

var TimeOut = 300;
var timer   = null;
var open_id  = "";
var old_id   = "";

function mopen(id) {
  mcancel();
  open_id = id;
  timer = window.setTimeout(handle_timeout, TimeOut);
}

function mclose() {
  mcancel();
  open_id = "";
  timer = window.setTimeout(handle_timeout, TimeOut);
}

function mcancel() {
  if (timer) {
    window.clearTimeout(timer);
    timer = null;
  }
}

function handle_timeout() {
  mcancel();
  mreopen(open_id);
}

function mcancelclose() {
  if (timer && open_id == "")
    mcancel();
}

function _mclose() {
  mcancel();
  mreopen("");
}

function mreopen(id) {
  var menu, old_menu;
  var layer, shim;
  var subid, hide;
  var offsetLeft = 0, offsetTop = 0;

  /* �V�������j���[��\�� */
  menu = id.split("-");
  subid = "";
  for (var i = 1; i < menu.length; i++) {
    subid += "-" + menu[i];
    layer = document.getElementById(menu[0] + subid);
    if (layer) {
      layer.style.visibility = 'visible'
      offsetLeft += layer.offsetLeft;
      offsetTop  += layer.offsetTop;
    }
  }

  /* �Â����j���[���B�� */
  old_menu = old_id.split("-");
  subid = "";
  hide = 0;
  for (var i = 1; i < old_menu.length; i++) {
    subid += "-" + old_menu[i];
    if (i >= menu.length || old_menu[i] != menu[i])
      hide = 1;
    if (hide && (layer = document.getElementById(old_menu[0] + subid)))
      layer.style.visibility = 'hidden'
  }

  /* IE6�̃o�O��� */
  /* TODO ���[�̃��j���[�����łȂ��S���j���[��IFRAME��p�ӂ��ׂ� */
  if (isMSIE6() && (shim = document.getElementById("shim"))) {
    layer = (id == "") ? null : document.getElementById(id);
    if (layer) {
      /* IFRAME�����j���[�ɏd�˂� */
      shim.style.left    = offsetLeft;
      shim.style.top     = offsetTop + 1;	/* XXX ������ */
      shim.style.width   = layer.offsetWidth;
      shim.style.height  = layer.offsetHeight;
      shim.style.display = 'block';
    } else {
      shim.style.display = 'none';
    }
  }

  old_id = id;
}

document.onclick = _mclose;
