/*
 * JavaScript Library written by H.Nagano
 *   Copyright (C) 2006 Icom Inc.
 */

var Class = {
    create: function()
    {
        return function()
        {
            this.initialize.apply(this, arguments);
        }
    }
}

/*
 * StyleSheet操作用クラス
 */
var JsCSS = Class.create();

JsCSS.prototype = {

    /*
     * コンストラクター: ブラウザーの種類をチェックする
     */
    initialize: function()
    {
        this.browserName = "MSIE";
        this.browserVersion = "6.0";
        if (window.navigator.appName == "Microsoft Internet Explorer")
        {
            this.browserName = "MSIE";
            if (window.navigator.appVersion.match(/compatible; MSIE ([^;]+);/))
            {
                this.browserVersion = RegExp.$1;
            }
        }
        else if (window.navigator.appName == "Netscape")
        {
            if (window.navigator.userAgent.match(/Firefox\/(.+)$/))
            {
                this.browserName = "Firefox";
                this.browserVersion = RegExp.$1;
            }
            else if (window.navigator.userAgent.match(/Netscape\d+\/(.+)$/))
            {
                this.browserName = "Netscape";
                this.browserVersion = RegExp.$1;
            }
        }
    },

    /*
     * 指定IDの部分をtextで置き替える (htmlタグ使用可能)
     *
     *   CSS = new JsCSS();
     *   CSS.setInnerHtml('test', '<h1>Hello</h1>');
     *
     *   <span id="test">ここが置き換わる</span>
     *
     */
    setInnerHtml: function(id, text)
    {
        if (!document.getElementById)
            return;
        if (!(elem = document.getElementById(id)))
            return;
        elem.innerHTML = text;
    },

    /*
     * 指定IDの部分をtextで置き替える (htmlタグ使用不可能)
     *
     *   CSS = new JsCSS();
     *   CSS.setInnerText('test', 'Hello');
     *
     *   <span id="test">ここが置き換わる</span>
     *
     */
    setInnerText: function(id, text)
    {
        if (!document.getElementById)
            return;
        if (!(elem = document.getElementById(id)))
            return;
        if (this.browserName != "MSIE")
        {
            text = text.replace(/</gm, "&lt;");
            text = text.replace(/>/gm, "&gt;");
            elem.innerHTML = text;
        }
        else
        {
            elem.innerText = text;
        }
    },

    /*
     * 指定IDの部分の表示/非表示を切り替える
     *
     *   CSS = new JsCSS();
     *   CSS.setVisible('test', false);
     *
     *   <span id="test">ここが見えなくなる</span>
     *
     */
    setVisible: function(id, visible)
    {
        if (!document.getElementById)
            return;
        if (!(elem = document.getElementById(id)))
            return;
        if (visible == true)
            elem.style.display = '';
        else
            elem.style.display = 'none';
    }
}


/*
 * Form操作用クラス
 */
var JsForm = Class.create();

JsForm.prototype = {

    /*
     * コンストラクター
     */
    initialize: function()
    {
    },

    /*
     * 指定nameのoption部分をopts配列の物に取り換える
     *
     *   opts    = new Array();
     *   opts[0] = new Array("test1", "value1");
     *   opts[1] = new Array("test2", "value2");
     *   opts[3] = new Array("test3", "value3");
     *
     *   Form = new JsForm();
     *   Form.setOptions('test', opts, "value2");
     *
     *   <select name="test">
     *   <option>ここが変る</option>
     *   </select>
     *
     */
    setOptions: function(name, opts, sel)
    {
        if (!document.getElementsByName)
            return false;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return false;

        var elem = elems[0];
        var ret = true;
        var idx = -1;

        var value = sel;
	if (sel == null)
		value = this.getOptionValue(name);

        for (i = 0; i < opts.length; i++)
        {
            elem.options[i] = new Option(opts[i][0], opts[i][1]);
            if (opts[i][1] == value)
            {
                idx = i;
                ret = false;
                elem.options[i].defaultSelected = true;
            }
        }
        if (idx < 0)
        {
            idx = 0;
        }
        elem.options.length = opts.length;
        elem.selectedIndex = idx;

        return ret;
    },

    /*
     * 指定nameのoptionで現在選択されている値を返す
     *
     *   Form = new JsForm();
     *   Form.getOptionValue('test');    // "value2"が返る
     *
     *   <select name="test">
     *   <option value="value1">test1</option>
     *   <option value="value2" selected>test2</option>
     *   <option value="value3">test3</option>
     *   </select>
     *
     */
    getOptionValue: function(name)
    {
        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return null;
        return elems[0].options[elems[0].selectedIndex].value;
    },

    /*
     * 指定nameのoptionで現在選択されている表示値を返す
     *
     *   Form = new JsForm();
     *   Form.getOptionText('test');    // "test2"が返る
     *
     *   <select name="test">
     *   <option value="value1">test1</option>
     *   <option value="value2" selected>test2</option>
     *   <option value="value3">test3</option>
     *   </select>
     *
     */
    getOptionText: function(name)
    {
        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return null;
        return elems[0].options[elems[0].selectedIndex].text;
    },

    /*
     * 指定nameのoptionでデフォルト選択されている値を返す
     *
     *   Form = new JsForm();
     *   Form.getOptionDefaultValue('test');    // "value2"が返る
     *
     *   <select name="test">
     *   <option value="value1">test1</option>
     *   <option value="value2" selected>test2</option>
     *   <option value="value3">test3</option>
     *   </select>
     *
     */
    getOptionDefaultValue: function(name)
    {
        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return null;
        for (i = 0; i < elems[0].options.length; i++)
        {
            if (elems[0].options[i].defaultSelected)
                return elems[0].options[i].value;
        }
        return null;
    },

    /*
     * 指定nameのoptionでデフォルト選択されている表示値を返す
     *
     *   Form = new JsForm();
     *   Form.getOptionDefaultText('test');    // "test2"が返る
     *
     *   <select name="test">
     *   <option value="value1">test1</option>
     *   <option value="value2" selected>test2</option>
     *   <option value="value3">test3</option>
     *   </select>
     *
     */
    getOptionDefaultText: function(name)
    {
        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return null;
        for (i = 0; i < elems[0].options.length; i++)
        {
            if (elems[0].options[i].defaultSelected)
                return elems[0].options[i].text;
        }
        return null;
    },

    /*
     * 指定nameのラジオボタンで現在選択されている値を返す
     *
     *   Form = new JsForm();
     *   Form.getRadioBox('test');    // "value2"が返る
     *
     *   <input type="radio" name="test" value="value1">
     *   <input type="radio" name="test" value="value2" checked>
     *
     */
    getRadioBox: function(name)
    {
        var i, value;

        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)))
            return null;
        for (i = 0; i < elems.length; i++)
        {
            if (elems[i].checked)
            {
                return elems[i].value;
            }
        }
        return null;
    },

    /*
     * 指定nameのチェックボックスが現在選択されているかを返す
     *
     *   Form = new JsForm();
     *   Form.getChecked('test');    // trueが返る
     *
     *   <input type="checkbox" name="test" value="value1" checked>
     *
     */
    getChecked: function(name)
    {
        var i, value;

        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)))
            return null;
        return elems[0].checked;
    },

    /*
     * 指定nameのチェックボックスの選択状態を変更する
     *
     *   Form = new JsForm();
     *   Form.setChecked('test', true);    // チェック状態にする
     *
     *   <input type="checkbox" name="test" value="value1">
     *
     */
    setChecked: function(name, checked)
    {
        var i, value;

        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)))
            return null;
        elems[0].checked = checked;
    },

    /*
     * 指定nameのエレメントの値を変更する
     *
     *   Form = new JsForm();
     *   Form.setValue('test', 'xyz');    // 'xyz'に変更
     *
     *   <input type="text" name="test" value="value1">
     *
     */
    setValue: function(name, value)
    {
        if (!document.getElementsByName)
            return;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return;
        elems[0].value = value;
    },

    /*
     * 指定nameのエレメントの値を返す
     *
     *   Form = new JsForm();
     *   Form.getValue('test');    // 'value1'が返る
     *
     *   <input type="text" name="test" value="value1">
     *
     */
    getValue: function(name)
    {
        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return null;
        return elems[0].value;
    },

    /*
     * 指定nameのエレメントを編集許可/不可を変更する
     *
     *   Form = new JsForm();
     *   Form.setDisabled('test', true);    // 編集不可にする
     *
     *   <input type="text" name="test" value="value1">
     *
     */
    setDisabled: function(name, disabled)
    {
        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return null;
        elems[0].disabled = disabled;
    },

    /*
     * 指定nameのエレメントを編集許可/不可を返す
     *
     *   Form = new JsForm();
     *   Form.setDisabled('test');    // falseが返る
     *
     *   <input type="text" name="test" value="value1">
     *
     */
    getDisabled: function(name, disabled)
    {
        if (!document.getElementsByName)
            return null;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return null;
        return elems[0].disabled;
    },

    /*
     * 指定nameのエレメントの長さを返す
     *
     *   Form = new JsForm();
     *   Form.getValueLength('test');    // 6が返る
     *
     *   <input type="text" name="test" value="value1">
     *
     */
    getValueLength: function(name)
    {
        if (!document.getElementsByName)
            return 0;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return 0;
        return elems[0].value.length;
    },

    /*
     * 指定nameのエレメントの最大長さを変更する
     *
     *   Form = new JsForm();
     *   Form.setMaxLength('test', 8);    // 最大8文字に設定する
     *
     *   <input type="text" name="test" value="value1">
     *
     */
    setMaxLength: function(name, len)
    {
        if (!document.getElementsByName)
            return;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return;
        elems[0].maxLength = len;
    },

    /*
     * 指定nameのエレメントの最大長さを返す
     *
     *   Form = new JsForm();
     *   Form.getMaxLength('test');    // 12が返る
     *
     *   <input type="text" name="test" maxlength="12" value="value1">
     *
     */
    getMaxLength: function(name)
    {
        if (!document.getElementsByName)
            return 0;
        if (!(elems = document.getElementsByName(name)) || elems.length == 0)
            return 0;
        return elems[0].maxLength;
    }
}
