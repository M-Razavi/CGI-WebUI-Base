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
 * StyleSheet����p�N���X
 */
var JsCSS = Class.create();

JsCSS.prototype = {

    /*
     * �R���X�g���N�^�[: �u���E�U�[�̎�ނ��`�F�b�N����
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
     * �w��ID�̕�����text�Œu���ւ��� (html�^�O�g�p�\)
     *
     *   CSS = new JsCSS();
     *   CSS.setInnerHtml('test', '<h1>Hello</h1>');
     *
     *   <span id="test">�������u�������</span>
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
     * �w��ID�̕�����text�Œu���ւ��� (html�^�O�g�p�s�\)
     *
     *   CSS = new JsCSS();
     *   CSS.setInnerText('test', 'Hello');
     *
     *   <span id="test">�������u�������</span>
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
     * �w��ID�̕����̕\��/��\����؂�ւ���
     *
     *   CSS = new JsCSS();
     *   CSS.setVisible('test', false);
     *
     *   <span id="test">�����������Ȃ��Ȃ�</span>
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
 * Form����p�N���X
 */
var JsForm = Class.create();

JsForm.prototype = {

    /*
     * �R���X�g���N�^�[
     */
    initialize: function()
    {
    },

    /*
     * �w��name��option������opts�z��̕��Ɏ�芷����
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
     *   <option>�������ς�</option>
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
     * �w��name��option�Ō��ݑI������Ă���l��Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getOptionValue('test');    // "value2"���Ԃ�
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
     * �w��name��option�Ō��ݑI������Ă���\���l��Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getOptionText('test');    // "test2"���Ԃ�
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
     * �w��name��option�Ńf�t�H���g�I������Ă���l��Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getOptionDefaultValue('test');    // "value2"���Ԃ�
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
     * �w��name��option�Ńf�t�H���g�I������Ă���\���l��Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getOptionDefaultText('test');    // "test2"���Ԃ�
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
     * �w��name�̃��W�I�{�^���Ō��ݑI������Ă���l��Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getRadioBox('test');    // "value2"���Ԃ�
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
     * �w��name�̃`�F�b�N�{�b�N�X�����ݑI������Ă��邩��Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getChecked('test');    // true���Ԃ�
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
     * �w��name�̃`�F�b�N�{�b�N�X�̑I����Ԃ�ύX����
     *
     *   Form = new JsForm();
     *   Form.setChecked('test', true);    // �`�F�b�N��Ԃɂ���
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
     * �w��name�̃G�������g�̒l��ύX����
     *
     *   Form = new JsForm();
     *   Form.setValue('test', 'xyz');    // 'xyz'�ɕύX
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
     * �w��name�̃G�������g�̒l��Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getValue('test');    // 'value1'���Ԃ�
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
     * �w��name�̃G�������g��ҏW����/�s��ύX����
     *
     *   Form = new JsForm();
     *   Form.setDisabled('test', true);    // �ҏW�s�ɂ���
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
     * �w��name�̃G�������g��ҏW����/�s��Ԃ�
     *
     *   Form = new JsForm();
     *   Form.setDisabled('test');    // false���Ԃ�
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
     * �w��name�̃G�������g�̒�����Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getValueLength('test');    // 6���Ԃ�
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
     * �w��name�̃G�������g�̍ő咷����ύX����
     *
     *   Form = new JsForm();
     *   Form.setMaxLength('test', 8);    // �ő�8�����ɐݒ肷��
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
     * �w��name�̃G�������g�̍ő咷����Ԃ�
     *
     *   Form = new JsForm();
     *   Form.getMaxLength('test');    // 12���Ԃ�
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
