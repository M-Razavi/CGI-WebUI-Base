/* This script and many more are available free online at
The JavaScript Source!! http://javascript.internet.com
Created by: Konstantin Jagello | http://javascript-array.com/ */

var objSubMenu='',strMenuPath='0',objDiv,intMenuTC,intMenuFC;
var intMenuIntervalId,strMenuOldClass,isMenuFadeing=0,objSubMenuFadeOld=0;
var intMenuWidth=205;

function findPosX(obj)
{
  var curleft = 0;
  if(obj.offsetParent)
    while(1)
  {
    curleft += obj.offsetLeft;
    if(!obj.offsetParent)
      break;
    obj = obj.offsetParent;
  }
  else if(obj.x)
    {
    curleft += obj.x;
  }
  return curleft;
}

function findPosY(obj)
{
  var curtop = 0;
  if(obj.offsetParent)
    while(1)
  {
    curtop += obj.offsetTop;
    if(!obj.offsetParent)
      break;
    obj = obj.offsetParent;
  }
  else if(obj.y)
    curtop += obj.y;
  return curtop;
}

function getBrowser()
{
  if (navigator.userAgent.indexOf('MSIE')>0)
    {
    return 'ie';
  }
  else
    {
    return 'ff';
  }
}

function menuMouseOver(objTd,intIndex)
{
  clearInterval(intMenuIntervalId);
  if (typeof intIndex=='undefined' || intIndex=='0')
    {
    intIndex=-1;
  }
  objTr=objTd;//.parentNode;
  objDiv=objTr;
  //get cur DIV
  do
  {
    objDiv=objDiv.parentNode;
  }while (!(objDiv.tagName=='DIV' && objDiv.id!=undefined));
  intDivId=objDiv.id.slice(7,objDiv.id.length);
  //change class (mouseOver)
  strMenuOldClass=objTd.className;
  if (intDivId==0)
    {
    //objTd.className='link active';			//menu
  }
  else
    {
    //objTd.className='link active';				//submenu
  }
  if (menuHidePrevPath(intDivId,intIndex)==0)
    {
    if (intIndex!=-1)
      {
      strMenuPath=strMenuPath+','+intIndex;			//append curDiv to menuPath
      intMenuTC=0;
      intMenuFC=1;

      objSubMenu=document.getElementById('subMenu'+intIndex);

      if (objSubMenuFadeOld!=0 && objSubMenuFadeOld!=objSubMenu)		//incomplete fade
        {
        if (objSubMenuFadeOld.style.display!='none')
          {
          objSubMenuFadeOld.style.opacity = 1;
          objSubMenuFadeOld.style.filter = 'alpha(opacity='+(100)+')';
        }
      }
      if(objSubMenu!='undefined' && objSubMenu!=null && objSubMenu!='')
        {
        intBaseMX=0;
        intBaseMY=0;
        if(document.getElementById("igmm")!=null)
          {
          intBaseMX=findPosX(document.getElementById("igmm"));
          intBaseMY=findPosY(document.getElementById("igmm"));
        }

        objSubMenu.style.top=findPosY(objTr)-intBaseMY+'px';

        if (getBrowser()=='ie')
          {
          objSubMenu.style.left=((findPosX(objTr)+intMenuWidth+3)+intBaseMX)+'px';
        }
        else
          {
          objSubMenu.style.left=((findPosX(objTr)+intMenuWidth+2)+intBaseMX)+'px';
        }

        objSubMenu.style.opacity = intMenuFC/10;
        objSubMenu.style.filter = 'alpha(opacity='+(intMenuFC*10)+')';
        objSubMenu.style.display='';
      }

      setTimeout('menuFadeIn()',1);
    }
  }
  return;
}

function menuHidePrevPath(intDivId,intNewSubMenu)
{
  //hide prevPath
  strMenuPathS=strMenuPath.split(',');
  intC=strMenuPathS.length-1;
  if (intC!=0)
    {
    //check and clear prevPath
    if (strMenuPathS[intC]==intNewSubMenu)
      {
      return 1;
    }
    do
    {
      if (strMenuPathS[intC]+0!=intDivId+0)
        {

        if(document.getElementById('subMenu'+strMenuPathS[intC])!='undefined' && document.getElementById('subMenu'+strMenuPathS[intC])!=null)
          {
          document.getElementById('subMenu'+strMenuPathS[intC]).style.display='none';

        }
        intC--;

      }
    }while (strMenuPathS[intC]+0!=intDivId+0 && intC+0!=0);
    strMenuPath='0';
    if (intC!=0)
      {
      for (intCS=1; intCS<=intC; intCS++)
        {
        strMenuPath=strMenuPath+','+strMenuPathS[intCS];
      }
    }
  }
  clearInterval(intMenuIntervalId);
  return 0;
}

function menuFadeIn()
{
  intMenuTC++;
  if (getBrowser()=='ie')
    {
    intMenuFC=intMenuFC+3;
  }
  else
    {
    intMenuFC=intMenuFC+1;
  }
  if(objSubMenu!='undefined' && objSubMenu!=null && objSubMenu!='')
    {
    objSubMenu.style.opacity = intMenuFC/10;
    objSubMenu.style.filter = 'alpha(opacity='+(intMenuFC*10)+')';
    objSubMenuFadeOld=objSubMenu;
  }
  if (intMenuFC<10)
    {
    isMenuFadeing=1;
    setTimeout('menuFadeIn()',20);
  }
  else
    {
    isMenuFadeing=0;
  }
  return;
}

function menuMouseOut(objTd)
{
  objTd.className=strMenuOldClass;
  intMenuIntervalId=setInterval('menuHidePrevPath(-10)',2000)
  return;
}

function checkForm(form) 
{
	if(form.newpasswd.value != "" && form.newpasswd.value == form.confirmpasswd.value) 
	{
		 re = /^[a-z0-9A-Z]+$/;
		if(!re.test(form.newpasswd.value)) 
		{
		 //alert("Error: password must contain at least one number (0-9)!");
		 alert("خطا: رمزعبور فقط شامل اعداد و حروف می تواند باشد.");
		 form.newpasswd.focus();
		 return false;
		}
		
		if(form.newpasswd.value.length < 6) 
		{
		 //alert("Error: Password must contain at least six characters!");
		 alert("خطا: رمزعبور بایستی حداقل 6 کاراکتر باشد.");
		 form.newpasswd.focus();
		 return false;
		} 
		
		re = /[0-9]/;
		if(!re.test(form.newpasswd.value)) 
		{
		 //alert("Error: password must contain at least one number (0-9)!");
		 alert("خطا: رمزعبور بایستی حداقل شامل یک عدد (0-9) باشد.");
		 form.newpasswd.focus();
		 return false;
		}
		
		re = /[a-z]/;
		if(!re.test(form.newpasswd.value)) 
		{
		 //alert("Error: password must contain at least one lowercase letter (a-z)!");
		 alert("خطا: رمزعبور بایستی حداقل شامل یک حرف کوچک انگلیسی (a-z) باشد.");
		 form.newpasswd.focus();
		 return false;
		}
		
		re = /[A-Z]/;
		if(!re.test(form.newpasswd.value)) 
		{
		 //alert("Error: password must contain at least one uppercase letter (A-Z)!");
		 alert("خطا: رمزعبور بایستی حداقل شامل یک حرف بزرگ انگلیسی (A-Z) باشد.");
		 form.newpasswd.focus();
		 return false;
		} 
	} 
	else 
	{
		if(form.newpasswd.value == "")
		{
			alert("خطا: رمزعبور نمیتواند خالی باشد.");
		}
		if(form.newpasswd.value != form.confirmpasswd.value)
		{
			alert("خطا: رمزعبور و تکرار رمز بایستی یکسان باشند.");
		}
		 //alert("Error: Please check that you've entered and confirmed your password!");
		 form.newpasswd.focus();
		 return false;
	} 
	return true;
 } 
 
 

function addPaginationToGrid()
{
	var docUrl = document.URL;
	docUrl = docUrl.replace(/&amp;pageno=\d*/g,"");
	docUrl = docUrl.replace(/&pageno=\d*/g,"");
	
	var str=document.getElementById("Pagination").innerHTML; 
	while(str.search("@@docurl@") != -1)
	{
		str=str.replace("@@docurl@",docUrl);
		document.getElementById("Pagination").innerHTML=str;
	}
}

/*collapse expand single item
by Leo Charre & Jesse Fergusson
Internet Connection  2004 2005 ©
www.internetconnection.net


Usage:

place this in your HEAD tags:

	<script language="JavaScript" src="/WHEREINPATH/collapse_expand_single_item.js"></script>

Place this in your HTML

	<img src="/IMAGESDIR/u.gif" name="imgfirst" width="9" height="9" border="0" >
	<a  href="#first" onClick="shoh('first');" >Customer Support</a>

	<div style="display: none;" id="first" >
			
			With its friendly solutions-oriented 
			approach, our timely and knowledgeable Technical Support Staff are 
			completely at your disposal. Our Support Technicians are highly 
			trained on the inner workings of the Internet and its associated 
			technologies. Combined with effective troubleshooting techniques, 
			we can quickly identify and resolve technical issues whether they 
			are on our end or on the client end. 		      
	
	</div>


WHEREINPATH is where you are storing this script on your account
IMAGESDIR is where on your acoount you are storing the icons for (expanded collapsed)

*/

imgout=new Image(16,16);
imgin=new Image(16,16);

/////////////////BEGIN USER EDITABLE///////////////////////////////
//	imgout.src="/gfx/u.gif";
//	imgin.src="/gfx/d.gif";

	imgout.src="/cgi_htmls/images/u.gif";
	imgin.src="/cgi_htmls/images/d.gif";
///////////////END USER EDITABLE///////////////////////////////////

//this switches expand collapse icons
function filter(imagename,objectsrc){
	if (document.images){
		document.images[imagename].src=eval(objectsrc+".src");
	}
}

//show OR hide funtion depends on if element is shown or hidden
function shoh(id) 
{ 
	
	if (document.getElementById) { // DOM3 = IE5, NS6
		if (document.getElementById(id).style.display == "none"){
			document.getElementById(id).style.display = 'block';
			filter(("img"+id),'imgin');			
		} else {
			filter(("img"+id),'imgout');
			document.getElementById(id).style.display = 'none';			
		}	
	} else { 
		if (document.layers) {	
			if (document.id.display == "none"){
				document.id.display = 'block';
				filter(("img"+id),'imgin');
			} else {
				filter(("img"+id),'imgout');	
				document.id.display = 'none';
			}
		} else {
			if (document.all.id.style.visibility == "none"){
				document.all.id.style.display = 'block';
			} else {
				filter(("img"+id),'imgout');
				document.all.id.style.display = 'none';
			}
		}
	}
}

function checkEmptyFileUploader(uploaderName)
{
	uploader = document.getElementById(uploaderName);
	if(uploader.files.length > 0)
	{
		return true;
	}
		alert("فایلی برای آپلود انتخاب نشده است.");
		return false;
}