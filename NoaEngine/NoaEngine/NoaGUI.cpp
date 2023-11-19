#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include "Renderer.h"
#include "Debug.h"
#include "NoaGUI.h"
#include "Sprite.h"
#include "InputSystem.h"
#include "Screen.h"

namespace noa {

	extern std::shared_ptr<Renderer> renderer;

}

noa::SpriteFile CreateSpriteFromBitmap(FT_Bitmap* bitmap)
{
	noa::SpriteFile sprite;

	sprite.width = bitmap->width;
	sprite.height = bitmap->rows;
	sprite.images.resize(sprite.width * sprite.height, ERRORCOLOR);

	if (bitmap->width == 0 || bitmap->rows == 0) {
		sprite.x = 0;
		sprite.y = 0;
		return sprite;
	}

	for (unsigned int y = 0; y < bitmap->rows; y++) {
		for (unsigned int x = 0; x < bitmap->width; x++) {

			uint8_t pixelValue = bitmap->buffer[y * (bitmap->width) + x];
			uint32_t pixelColor = noa::RGBA(255,255,255,pixelValue);

			sprite.images[y * sprite.width + x] = pixelColor;
		}
	}

	sprite.x = 0;
	sprite.y = 0;

	return sprite;
}

noa::FontAsset::FontAsset(const char* ttfPath, int size)
{

	this->size = size;

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		Debug::Error("Init FreeType failed");
		exit(-1);
	}

	FT_Face face;
	if (FT_New_Face(ft, ttfPath, 0, &face)) {
		Debug::Error("Init FT_Face failed");
		exit(-1);
	}

	FT_Set_Pixel_Sizes(face, 0, size);


	std::wstring chineseChar;
		chineseChar.append(L"Ò»ÒÒ¶şÊ®¶¡³§Æß²·ÈËÈë°Ë¾Å¼¸¶ùÁËÁ¦ÄËµ¶ÓÖÈıÓÚ¸É¿÷Ê¿¹¤ÍÁ²Å´çÏÂ´óÕÉÓëÍòÉÏĞ¡¿Ú½íÉ½Ç§Æò´¨ÒÚ¸ö");
		chineseChar.append(L"É×¾Ã·²¼°Ï¦ÍèÃ´¹ãÍöÃÅÒåÖ®Ê¬¹­¼ºÒÑ×ÓÎÀÒ²Å®·ÉÈĞÏ°²æÂíÏç·áÍõ¾®¿ª·òÌìÎŞÔª×¨ÔÆÔúÒÕÄ¾ÎåÖ§Ìü²»");
		chineseChar.append(L"Ì«È®ÇøÀúÓÈÓÑÆ¥³µ¾ŞÑÀÍÍ±È»¥ÇĞÍßÖ¹ÉÙÈÕÖĞ¸Ô±´ÄÚË®¼ûÎçÅ£ÊÖÃ«ÆøÉı³¤ÈÊÊ²Æ¬ÆÍ»¯³ğ±ÒÈÔ½ö½ï×¦·´");
		chineseChar.append(L"½é¸¸´Ó½ñĞ×·Ö·¦¹«²ÖÔÂÊÏÎğÇ··çµ¤ÔÈÎÚ·ï¹´ÎÄÁù·½»ğÎª¶·Òä¶©¼Æ»§ÈÏĞÄ³ßÒı³ó°Í¿×¶Ó°ìÒÔÔÊÓèÈ°Ë«");
		chineseChar.append(L"Êé»ÃÓñ¿¯Ê¾Ä©Î´»÷´òÇÉÕıÆË°Ç¹¦ÈÓÈ¥¸ÊÊÀ¹Å½Ú±¾Êõ¿É±û×óÀ÷ÓÒÊ¯²¼ÁúÆ½ÃğÔş¶«¿¨±±Õ¼Òµ¾ÉË§¹éÇÒµ©");
		chineseChar.append(L"Ä¿Ò¶¼×Éê¶£µçºÅÌïÓÉÊ·Ö»ÑëĞÖµğ½ĞÁíß¶Ì¾ËÄÉúÊ§ºÌÇğ¸¶ÕÌ´úÏÉÃÇÒÇ°××ĞËû³â¹Ïºõ´ÔÁîÓÃË¦Ó¡ÀÖ¾ä´Ò");
		chineseChar.append(L"²á·¸Íâ´¦¶¬ÄñÎñ°ü¼¢Ö÷ÊĞÁ¢ÉÁÀ¼°ëÖ­»ãÍ·ººÄşÑ¨ËüÌÖĞ´ÈÃÀñÑµ±ØÒéÑ¶¼ÇÓÀË¾ÄáÃñ³öÁÉÄÌÅ«¼ÓÕÙÆ¤±ß");
		chineseChar.append(L"·¢ÔĞÊ¥¶ÔÌ¨Ã¬¾ÀÄ¸Ó×Ë¿Ê½ĞÌ¶¯¿¸ËÂ¼ª¿Û¿¼ÍĞÀÏÖ´¹®»øÀ©É¨µØÑï³¡¶ú¹²Ã¢ÑÇÖ¥ĞàÆÓ»úÈ¨¹ı³¼ÔÙĞ­Î÷Ñ¹");
		chineseChar.append(L"ÑáÔÚÓĞ°Ù´æ¶øÒ³½³¿ä¶á»Ò´ïÁĞËÀ³É¼Ğ¹ìĞ°»®Âõ±ÏÖÁ´ËÕêÊ¦³¾¼âÁÓ¹âµ±ÔçÍÂÏÅ³æÇúÍÅÍ¬µõ³ÔÒòÎüÂğÓì");
		chineseChar.append(L"·«Ëê»ØÆñ¸ÕÔòÈâÍøÄêÖìÏÈ¶ªÉàÖñÇ¨ÇÇÎ°´«Æ¹ÅÒĞİÎé·üÓÅ·¥ÑÓ¼şÈÎÉË¼Û·İ»ªÑö·Â»ïÎ±×ÔÑªÏòËÆºóĞĞÖÛ");
		chineseChar.append(L"È«»áÉ±ºÏÕ×ÆóÖÚÒ¯É¡´´¼¡¶äÔÓÎ£Ñ®Ö¼¸º¸÷Ãû¶àÕùÉ«×³³å±ù×¯ÇìÒàÁõÆë½»´ÎÒÂ²ú¾ö³äÍı±ÕÎÊ´³Ñò²¢¹Ø");
		chineseChar.append(L"Ã×µÆÖİº¹ÎÛ½­³ØÌÀÃ¦ĞËÓîÊØÕ¬×Ö°²½²¾üĞíÂÛÅ©·íÉè·ÃÑ°ÄÇÑ¸¾¡µ¼ÒìËïÕóÑôÊÕ½×Òõ·À¼éÈç¸¾ºÃËıÂèÏ·");
		chineseChar.append(L"Óğ¹Û»¶ÂòºìÏË¼¶Ô¼¼Í³ÛÑ²ÊÙÅªÂóĞÎ½ø½äÍÌÔ¶Î¥ÔË·ö¸§Ì³¼¼»µÈÅ¾ÜÕÒÅú³¶Ö·×ß³­°Ó¹±¹¥³àÕÛ×¥°çÇÀĞ¢");
		chineseChar.append(L"¾ùÅ×Í¶·Ø¿¹¿Ó·»¶¶»¤¿ÇÖ¾Å¤¿éÉù°Ñ±¨È´½ÙÑ¿»¨ÇÛ·Ò²Ô·¼ÑÏÂ«ÀÍ¿ËËÕ¸Ë¸Ü¶Å²Ä´åĞÓ¼«ÀîÑîÇó¸üÊø¶¹Á½");
		chineseChar.append(L"ÀöÒ½³½Àø·ñ»¹¼ßÀ´Á¬²½¼áºµ¶¢³ÊÊ±ÎâÖúÏØÀï´ôÔ°¿õÎ§Ñ½¶Ö×ãÓÊÄĞÀ§³³´®Ô±Ìı·Ô´µÎØ°Éºğ±ğ¸ÚÕÊ²ÆÕë");
		chineseChar.append(L"¶¤¸æÎÒÂÒÀûÍºĞãË½Ã¿±ø¹ÀÌåºÎµ«Éì×÷²®ÁæÓ¶µÍÄã×¡Î»°éÉíÔí·ğ½ü³¹ÒÛ·µÓàÏ£×ø¹ÈÍ×º¬ÁÚ²í¸Î¶Ç³¦¹ê");
		chineseChar.append(L"Ãâ¿ñÓÌ½ÇÉ¾ÌõÂÑµºÓ­·¹ÒûÏµÑÔ¶³×´Ä¶¿ö´²¿âÁÆÓ¦ÀäÕâĞòĞÁÆúÒ±ÍüÏĞ¼äÃÆÅĞÔî²ÓµÜÍôÉ³ÆûÎÖ·º¹µÃ»Éò");
		chineseChar.append(L"³Á»³ÓÇ¿ìÍêËÎºêÀÎ¾¿ÇîÔÖÁ¼Ö¤ÆôÆÀ²¹³õÉçÊ¶ËßÕï´ÊÒë¾ıÁé¼´²ãÄòÎ²³Ù¾Ö¸ÄÕÅ¼É¼ÊÂ½°¢³Â×è¸½ÃîÑı·Á");
		chineseChar.append(L"Å¬ÈÌ¾¢¼¦Çı´¿É´ÄÉ¸Ù²µ×İ·×Ö½ÎÆ·ÄÂ¿Å¦·îÍæ»·ÎäÇàÔğÏÖ±í¹æÄ¨Â£°Î¼ğµ£Ì¹Ñº³é¹ÕÍÏÅÄÕß¶¥²ğÓµµÖ¾Ğ");
		chineseChar.append(L"ÊÆ±§À¬À­À¹°èĞÒÕĞÆÂÅû²¦ÔñÌ§ÆäÈ¡¿àÈôÃ¯Æ»ÃçÓ¢·¶Ö±ÇÑ¾¥Ã©ÁÖÖ¦±­¹ñÎö°åËÉÇ¹¹¹½ÜÊöÕíÉ¥»òÎÔÊÂ´Ì");
		chineseChar.append(L"ÔæÓêÂô¿óÂë²Ş±¼Ææ·ÜÌ¬Å·Â¢ÆŞºäÇê×ªÕ¶ÂÖÈíµ½·ÇÊå¿Ï³İĞ©»¢Â²ÉöÏÍÉĞÍú¾ß¹ûÎ¶À¥¹ú²ı³©Ã÷Ò×°ºµä¹Ì");
		chineseChar.append(L"ÖÒ¸ÀºôÃùÓ½ÄØ°¶ÑÒÌûÂŞÖÄÁë¿­°Ü··¹ºÍ¼µöÖÆÖª´¹ÄÁÎï¹Ô¹Î¸ÑºÍ¼¾Î¯¼ÑÊÌ¹©Ê¹Àı°æÖ¶Õì²àÆ¾ÇÈÅå»õÒÀ");
		chineseChar.append(L"µÄÆÈÖÊĞÀÕ÷ÍùÅÀ±Ë¾¶ËùÉá½ğÃü¸«°Ö²ÉÊÜÈéÌ°ÄîÆ¶·ô·ÎÖ«Ö×ÕÍÅó¹É·Ê·şĞ²ÖÜ»èÓãÍÃºüºö¹·±¸ÊÎ±¥ËÇ±ä");
		chineseChar.append(L"¾©ÏíµêÒ¹Ãí¸®µ×¼Á½¼·Ï¾»Ã¤·Å¿ÌÓıÕ¢ÄÖÖ£È¯¾íµ¥³´´¶¿»Ñ×Â¯Ä­Ç³·¨Ğ¹ºÓÕ´ÀáÓÍ²´ÑØÅİ×¢ĞºÓ¾Äà·Ğ²¨");
		chineseChar.append(L"ÆÃÔóÖÎ²ÀĞÔÅÂÁ¯¹ÖÑ§±¦×Ú¶¨ÒËÉóÖæ¹Ù¿ÕÁ±ÊµÊÔÀÉÊ«¼ç·¿³Ï³ÄÉÀÊÓ»°µ®Ñ¯¸ÃÏê½¨ËàÂ¼Á¥¾Ó½ìË¢ÇüÏÒ³Ğ");
		chineseChar.append(L"ÃÏ¹ÂÉÂ½µÏŞÃÃ¹Ã½ãĞÕÊ¼¼İ²Î¼èÏßÁ·×éÏ¸Ê»Ö¯ÖÕ×¤ÍÕÉÜ¾­¹á×à´º°ïÕä²£¶¾ĞÍ¹Ò·â³ÖÏî¿å¿æ³ÇÄÓÕş¸°ÕÔ");
		chineseChar.append(L"µ²Í¦À¨Ë©Ê°ÌôÖ¸µæÕõ¼·Æ´ÍÚ°´»ÓÅ²Ä³Éõ¸ï¼öÏï´ø²İ¼ë²è»ÄÃ£µ´ÈÙ¹ÊºúÄÏÒ©±ê¿İ±ú¶°Ïà²é°ØÁøÖùÊÁÀ¸");
		chineseChar.append(L"Ê÷ÒªÏÌÍşÍáÑĞ×©ÀåºñÆö¿³ÃæÄÍË£Ç£²ĞÑêÇáÑ»½Ô±³Õ½µãÁÙÀÀÊúÊ¡Ï÷³¢ÊÇÅÎÕ£ºåÏÔÑÆÃ°Ó³ĞÇ×òÎ·Å¿Î¸¹ó");
		chineseChar.append(L"½çºçÏºÒÏË¼ÂìËäÆ·ÑÊÂî»©ÔÛÏì¹şÒ§¿ÈÄÄÌ¿Ï¿·£¼úÌù¹Ç³®ÖÓ¸ÖÔ¿¹³Ğ¶¸×°İ¿´¾ØÔõÉüÑ¡ÊÊÃëÏãÖÖÇï¿ÆÖØ");
		chineseChar.append(L"¸´¸Í¶Î±ãÁ©´ûË³ĞŞ±£´ÙÎê¼óË×·ıĞÅ»ÊÈª¹íÇÖ×·¿¡¶Ü´ıÂÉºÜĞëĞğ½£ÌÓÊ³Åèµ¨Ê¤°ûÅÖÂöÃãÏÁÊ¨¶À½ÆÓüºİ");
		chineseChar.append(L"Ã³Ô¹¼±ÈÄÊ´½È±ıÍä½«½±°§Í¤ÁÁ¶È¼£Í¥´¯·èÒß°Ì×ËÇ×ÒôµÛÊ©ÎÅ·§¸ó²îÑøÃÀ½ªÅÑËÍÀàÃÔÇ°Ê×Äæ×ÜÁ¶Õ¨ÅÚ");
		chineseChar.append(L"ÀÃÌê½àºéÈ÷½½×Ç¶´²âÏ´»îÅÉÇ¢È¾¼ÃÑóÖŞ»ëÅ¨½òºã»ÖÇ¡ÄÕºŞ¾Ù¾õĞûÊÒ¹¬ÏÜÍ»´©ÇÔ¿Í¹ÚÓï±â°À×æÉñ×£Îó");
		chineseChar.append(L"ÓÕËµËĞ¿ÑÍË¼ÈÎİÖç·Ñ¶¸Ã¼º¢³ıÏÕÔºÍŞÀÑÒÌÒö½¿Å­¼ÜºØÓ¯ÓÂµ¡ÈáÀİ°óÈŞ½áÈÆ½¾»æ¸øÂçÂæ¾ø½ÊÍ³¸ûºÄÑŞ");
		chineseChar.append(L"Ì©Öé°àËØ²ÏÍçÕµ·ËÀÌÔÔ²¶ÕñÔØ¸ÏÆğÑÎÉÓÄóÂñ×½À¦¾èËğ¶¼ÕÜÊÅ¼ñ»»ÍìÈÈ¿Öºø°¤³Üµ¢¹§Á«ÄªºÉ»ñ½ú¶ñÕæ");
		chineseChar.append(L"¿ò¹ğµµÍ©ÖêÇÅÌÒ¸ñĞ£ºËÑù¸ùË÷¸çËÙ¶ºÀõÅä³áÈè´½ÏÄ´¡ÆÆÔ­Ì×ÖğÁÒÊâ¹Ë½Î½Ï¶Ù±ĞÖÂ²ñ×ÀÂÇ¼à½ôµ³É¹Ãß");
		chineseChar.append(L"ÏşÑ¼»ÎÉÎÔÎÎÃÉÚ¿Ş¶÷»½°¡°¦°Õ·åÔ²Ôô»ßÇ®Ç¯×êÌúÁåÇ¦È±ÑõÌØÎşÔì³ËµĞ³Ó×â»ıÑíÖÈ³ÆÃØÍ¸±ÊĞ¦ËñÕ®½è");
		chineseChar.append(L"ÖµÒĞÇãµ¹ÌÈ¾ã³«ºò¸©±¶¾ë½¡³ôÉä¹ªÏ¢Í½Ğì½¢²Õ°ãº½Í¾ÄÃµù°®ËÌÎÌ´àÖ¬ĞØ¸ìÔà½ºÄÔÀêÀÇ·êÁôÖå¶öÁµ½°");
		chineseChar.append(L"½¬Ë¥¸ßÏ¯×¼×ù¼¹Ö¢²¡¼²ÌÛÆ£Ğ§ÀëÌÆ×ÊÁ¹Õ¾ÆÊ¾º²¿ÅÔÂÃĞóÔÄĞßÆ¿È­·ÛÁÏÒæ¼æ¿¾ºæ·³ÉÕÖòÑÌµİÌÎÕãÀÔ¾Æ");
		chineseChar.append(L"ÉæÏûºÆº£Í¿Ô¡¸¡Á÷ÈóÀË½şÕÇÌÌÓ¿ÎòÇÄ»ÚÔÃº¦¿í¼ÒÏüÑç±öÕ­ÈİÔ×°¸ÇëÀÊÖî¶ÁÉÈÍàĞäÅÛ±»Ïé¿ÎË­µ÷Ô©ÁÂ");
		chineseChar.append(L"Ì¸Òê°ş¿ÒÕ¹¾çĞ¼ÈõÁêÌÕÏİÅãÓéÄïÍ¨ÄÜÄÑÔ¤É£¾îĞåÑé¼ÌÇòÀíÅõ¶ÂÃèÓòÑÚ½İÅÅµô¶ÑÍÆÏÆÊÚ½ÌÌÍÂÓÅà½Ó¿Ø");
		chineseChar.append(L"Ì½¾İ¾òÖ°»ùÖøÀÕ»ÆÃÈÂÜ¾ú²ËÌÑ¾ÕÆ¼²¤ÓªĞµÃÎÉÒÃ·¼ìÊáÌİÍ°¾È¸±Æ±ÆİË¬ÁûÏ®Ê¢Ñ©¸¨Á¾ĞéÈ¸ÌÃ³£³×³¿Õö");
		chineseChar.append(L"ÃĞÑÛĞüÒ°À²Íí×Ä¾àÔ¾ÂÔÉßÀÛ³ª»¼Î¨ÑÂÕ¸³çÈ¦Í­²ùÒøÌğÀæÀçÒÆ±¿ÁıµÑ·ûµÚÃô×ö´üÓÆ³¥Å¼ÍµÄúÊÛÍ£Æ«¼Ù");
		chineseChar.append(L"µÃÏÎÅÌ´¬Ğ±ºĞ¸ëÏ¤Óû²ÊÁì½Å²±Á³ÍÑÏó¹»²ÂÖíÁÔÃ¨ÃÍÏÚ¹İ´Õ¼õºÁÂéÑ÷ºÛÀÈ¿µÓ¹Â¹µÁÕÂ¾¹ÉÌ×åĞıÍûÂÊ×Å");
		chineseChar.append(L"¸ÇÕ³´ÖÁ£¶Ï¼ôÊŞÇåÌíÁÜÑÍÇş½¥»ìÓæÌÔÒºµ­ÉîÆÅÁºÉøÇéÏ§²Ñµ¿¾åÌè¾ª²Ò¹ß¿Ü¼ÄËŞÒ¤ÃÜÄ±»Ñ»öÃÕ´ş¸ÒÍÀ");
		chineseChar.append(L"µ¯Ëæµ°Â¡Òş»éÉô¾±¼¨Ğ÷ĞøÆïÉşÎ¬Ãà³ñÂÌÇÙ°ßÌæ¿î¿°´îËşÔ½³ÃÇ÷³¬ÌáµÌ²©½ÒÏ²²å¾¾ËÑÖóÔ®²Ã¸éÂ§½ÁÎÕ");
		chineseChar.append(L"ÈàË¹ÆÚÆÛÁªÉ¢ÈÇÔá¸ğ¶­ÆÏ¾´´ĞÂä³¯¹¼¿û°ôÆåÖ²É­ÒÎ½·¿Ã¹÷ÃŞÅï×Ø»İ»ó±Æ³øÏÃÓ²È·ÑãÖ³ÁÑĞÛÔİÑÅ±²±¯");
		chineseChar.append(L"×Ï»Ô³¨ÉÍÕÆÇçÊî×îÁ¿Åç¾§À®Óöº°¾°¼ùµøÅÜÒÅÍÜÖëòÑºÈÎ¹´­ºí·ùÃ±¶ÄÅâºÚÖıÆÌÁ´ÏúËø³ú¹øĞâ·æÈñ¶ÌÖÇ");
		chineseChar.append(L"Ìº¶ìÊ£ÉÔ³ÌÏ¡Ë°¿ğµÈÖş²ßÉ¸Í²´ğ½îóİ°Á¸µÅÆ±¤¼¯½¹°ø´¢°Â½Ö³ÍÓùÑ­Í§Êæ·¬ÊÍÇİÀ°Æ¢Ç»Â³»«ºïÈ»²ö×°");
		chineseChar.append(L"Âù¾ÍÍ´Í¯À«ÉÆÏÛÆÕ·à×ğµÀÔøÑæ¸ÛºşÔüÊªÎÂ¿Ê»¬Íå¶ÉÓÎ×Ì¸È·ß»Å¶èÀ¢Óä¿®¸îº®¸»´ÜÎÑ´°±éÔ£¿ãÈ¹Ğ»Ò¥");
		chineseChar.append(L"Ç«ÊôÂÅÇ¿ÖàÊè¸ôÏ¶ĞõÉ©µÇ¶Ğ»º±àÆ­ÔµÈğ»êËÁÉãÃşÌî²«Ëú¹Ä°ÚĞ¯°áÒ¡¸ãÌÁÌ¯ËâÇÚÈµÀ¶Ä¹Ä»ÅîĞîÃÉÕôÏ×");
		chineseChar.append(L"½û³şÏë»±ÓÜÂ¥¸ÅÀµ³ê¸Ğ°­±®ËéÅöÍëÂµÀ×ÁãÎí±¢Êä¶½Áä¼ø¾¦Ë¯²Ç±ÉÓŞÅ¯ÃËĞª°µÕÕ¿çÌø¹òÂ·¸úÇ²¶ê·äÉ¤");
		chineseChar.append(L"ÖÃ×ïÕÖ´íÎıÂà´¸½õ¼ü¾â°«´Ç³í³î³ïÇ©¼ò»Ù¾ËÊó´ßÉµÏñ¶ãÎ¢ÓúÒ£ÑüĞÈ¸¹ÌÚÍÈ´¥½â½´ÌµÁ®ĞÂÔÏÒâÁ¸Êı¼å");
		chineseChar.append(L"ËÜ´ÈÃº»ÍÂúÄ®Ô´ÂËÀÄÌÏÏªÁï¹ö±õÁ»Ì²É÷ÓşÈû½÷¸£Èºµî±ÙÕÏÏÓ¼Şµş·ì²ø¾²±ÌÁ§Ç½Æ²¼Î´İ½ØÊÄ¾³ÕªË¤¾Û");
		chineseChar.append(L"±ÎÄ½ÄºÃïÄ£Áñ°ñÕ¥¸èÔâ¿áÄğËá´ÅÔ¸Ğè±×ÉÑ¿ÅËÔòßÀ¯Ó¬Ö©×¬ÇÂ¶ÍÎèÎÈËãÂá¹ÜÁÅ±ÇÆÇÃ²Ä¤²²°òÏÊÒÉÂø¹ü");
		chineseChar.append(L"ÇÃºÀ¸àÕÚ¸¯ÊİÀ±½ß¶ËÆì¾«Ç¸Ï¨ÈÛÆáÆ¯ÂşµÎÑİÂ©ÂıÕ¯Èü²ìÃÛÆ×ÄÛ´äĞÜµÊÂâËõ»ÛËºÈöÈ¤ÌË³Å²¥×²³·Ôö´Ï");
		chineseChar.append(L"Ğ¬½¶Êßºá²ÛÓ£ÏğÆ®´××íÕğÃ¹Â÷Ìâ±©Ï¹Ó°ÌßÌ¤²È×ÙµûºûÖöÄ«Õò¿¿µ¾Àè¸å¼ÚÏä¼ıÆª½©ÌÉÆ§µÂËÒÏ¥ÌÅÊìÄ¦");
		chineseChar.append(L"ÑÕÒãºı×ñÇ±³±¶®¶îÎ¿Åü²ÙÑàÊíĞ½±¡µßéÙÕûÈÚĞÑ²Í×ìÌãÆ÷ÔùÄ¬¾µÔŞÀºÑûºâÅòµñÄ¥Äı±æ±çÌÇ¸âÈ¼Ôè¼¤ÀÁ");
		chineseChar.append(L"±Ú±Ü½É´÷²Á¾Ï²ØËªÏ¼ÇÆµ¸ÂİËë·±±èÓ®Ôã¿·Ôï±ÛÒíÖè±Ş¸²±ÄÁ­·­Ó¥¾¯ÅÊ¶×²ü°ê±¬½®ÈÀÒ«Ôê½ÀÈÂ¼®Ä§¹à");
		chineseChar.append(L"´À°ÔÂ¶ÄÒ¹Ş~¡¤£¡@#£¤%¡­¡­&*£¨£©¡ª¡ª++-=¡¢|¡¾{}¡¿£»£º¡®¡°£¬¡¶¡£¡·/£¿*");

		chineseChar.append(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
		chineseChar.append(L" ~!@#$%^&*()-_=+[{}]\\|;:'\",<.>/?*");

		for (size_t i = 0; i < chineseChar.length(); i++) {
			if (FT_Load_Char(face, chineseChar[i], FT_LOAD_RENDER)) {
				continue;
			}

			Font* font = new Font(CreateSpriteFromBitmap(&face->glyph->bitmap));
			font->bearing.x = face->glyph->bitmap_left;
			font->bearing.y = face->glyph->bitmap_top;
			font->advance = face->glyph->advance.x;
			this->fonts[chineseChar[i]] = font;
		}

}

noa::Font* noa::FontAsset::GetFont(wchar_t c)
{
	if (this->fonts.count(c)<=0) 
	{
		return nullptr;
	}
	return fonts[c];
}

noa::Canvas::Canvas() {

}

noa::Canvas::~Canvas() {
	while (!groups.empty()) 
	{
		groups.pop();
	}
	for (auto & group:groupList)
	{
		group->Delete(group);
	}
}

void noa::Canvas::AddUIGroup(UIGroup * group) 
{
	if (group == nullptr) 
	{
		return;
	}
	group->index = groupList.size();
	this->groupList.push_back(group);
	if (groups.empty()) 
	{
		OpenGroup(group->index);
	}

}

void noa::Canvas::OpenGroup(size_t index) 
{
	if (index >=groupList.size()
		||groupList[index] == nullptr)
	{
		return;
	}
	groupList[index]->visiable = true;
	this->groups.push(groupList[index]);
}

void noa::Canvas::OpenGroup(const std::string& id) 
{
	UIGroup* group = this->GetGroupByID(id);
	if (!group) 
	{
		return;
	}
	OpenGroup(group->GetGroupIndex());
}

void noa::Canvas::OpenGroup(noa::UIGroup* group) 
{
	if (!group) 
	{
		return;
	}
	OpenGroup(group->GetGroupIndex());
}

void noa::Canvas::CloseGroup() {
	if (groups.size()<=1) 
	{
		return;
	}
	groups.top()->visiable = false;
	groups.pop();
}

noa::UIGroup* noa::Canvas::GetGroupByID(const std::string& id) 
{
	for (auto& group:groupList) 
	{
		if (group&&group->id == id) 
		{
			return group;
		}
	}
	return nullptr;
}

noa::Label* noa::Canvas::GetLabelByID(const std::string& id) 
{
	for (auto& group:groupList) 
	{
		if (!group) 
		{
			continue;
		}
		noa::Label* temp = group->GetLabelByID(id);
		if (temp) 
		{
			return temp;
		}
	}
	return nullptr;
}

noa::Image* noa::Canvas::GetImageByID(const std::string& id) 
{
	for (auto& group : groupList)
	{
		if (!group)
		{
			continue;
		}
		noa::Image* temp = group->GetImageByID(id);
		if (temp)
		{
			return temp;
		}
	}
	return nullptr;
}

noa::Button* noa::Canvas::GetButtonByID(const std::string& id) 
{
	for (auto& group : groupList)
	{
		if (!group)
		{
			continue;
		}
		noa::Button* temp = group->GetButtonByID(id);
		if (temp)
		{
			return temp;
		}
	}
	return nullptr;
}

void noa::Canvas::CanvasStart() {
	
}

void noa::Canvas::CanvasUpdate() 
{
	if (groups.empty())
	{
		return;
	}

	//¿ØÖÆÂß¼­Ö»¿ØÖÆÕ»¶¥
	if (groups.top() == nullptr) 
	{
		groups.pop();
	}
	else {
		groups.top()->Update();
	}

	for (auto& group:groupList) 
	{
		//Ô½ÍùÇ°µÄÔ½ÏÈ»æÖÆ
		if (group&&group->visiable)
		{
			group->Render();
		}
	}
}

noa::UIGroup::UIGroup(Canvas * canvas) 
{
	if (canvas)
	{
		canvas->AddUIGroup(this);
	}
}

noa::UIGroup::~UIGroup() {
	if (!this->uiComponent.empty())
	{
		//¶Ôcomponent½øĞĞÅÅĞò
		std::sort(uiComponent.begin(), uiComponent.end());
		auto last = std::unique(uiComponent.begin(), uiComponent.end());
		uiComponent.erase(last, uiComponent.end());

		for (auto& component : uiComponent)
		{
			if (component)
			{
				component->Delete(component);
			}
		}

	}
}

noa::UIGroup* noa::UIGroup::Create(noa::Canvas* canvas) 
{
	if (canvas == nullptr) 
	{
		return nullptr;
	}

	UIGroup* group = new UIGroup(canvas);
	return group;

}

void noa::UIGroup::Delete(UIGroup *& ptr) 
{
	delete this;
	ptr = nullptr;
}

noa::UIGroup& noa::UIGroup::SetID(const std::string& id) 
{
	this->id = id;
	return *this;
}

noa::UIGroup& noa::UIGroup::SetPosition(int x,int y) 
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::UIGroup* noa::UIGroup::Apply() {
	return this;
}

noa::Label* noa::UIGroup::GetLabelByID(const std::string& id) 
{
	for (auto& component:uiComponent) 
	{
		noa::Label* temp = dynamic_cast<noa::Label*>(component);
		if (temp&&temp->id == id) 
		{
			return temp;
		}
	}
	return nullptr;
}

noa::Image* noa::UIGroup::GetImageByID(const std::string& id) 
{
	for (auto& component : uiComponent)
	{
		noa::Image* temp = dynamic_cast<noa::Image*>(component);
		if (temp && temp->id == id)
		{
			return temp;
		}
	}
	return nullptr;
}

noa::Button* noa::UIGroup::GetButtonByID(const std::string& id) 
{
	for (auto& component : uiComponent)
	{
		noa::Button* temp = dynamic_cast<noa::Button*>(component);
		if (temp && temp->id == id)
		{
			return temp;
		}
	}
	return nullptr;
}

void noa::UIGroup::AddUIComponent(UIComponent* component) 
{
	if (component == nullptr)
	{
		return;
	}
	component->SetActive(true);
	uiComponent.push_back(component);
}

size_t noa::UIGroup::GetGroupIndex() {
	return this->index;
}

void noa::UIGroup::Start() {
	for (auto& component : uiComponent)
	{
		if (component != nullptr)
		{
			component->Start();
		}
	}
}

void noa::UIGroup::Update() 
{
	//ÏÔÊ¾±³¾°
	for (auto& component : uiComponent)
	{
		if (component == nullptr || !component->GetActive())
		{
			continue;
		}
		component->fatherTransform = this->transform;
		component->Update();
	}
}

void noa::UIGroup::Render() {
	//ÏÔÊ¾±³¾°
	for (auto& component : uiComponent)
	{
		if (component == nullptr || !component->GetActive())
		{
			continue;
		}
		component->fatherTransform = this->transform;
		component->Render();
	}
}

noa::UICanvasActor::UICanvasActor(Scene* scene) :Actor(scene), Canvas()
{

}

noa::UICanvasActor::~UICanvasActor()
{
	
}

noa::UICanvasActor* noa::UICanvasActor::Create(Scene* scene)
{
	return noa::NObject<UICanvasActor>::Create(scene);
}

void noa::UICanvasActor::Start()
{
	Canvas::CanvasStart();
}

void noa::UICanvasActor::Update()
{
	Canvas::CanvasUpdate();
}

noa::UICanvasComponent::UICanvasComponent(Actor* actor) :ActorComponent(actor)
{

}

noa::UICanvasComponent::~UICanvasComponent() {
	
}

noa::UICanvasComponent* noa::UICanvasComponent::Create(Actor * actor) 
{
	return noa::NObject<UICanvasComponent>::Create(actor);
}

void noa::UICanvasComponent::Start() {
	Canvas::CanvasStart();
}

void noa::UICanvasComponent::Update() {
	Canvas::CanvasUpdate();
}

noa::UIComponent::UIComponent(noa::UIGroup* group)
{
	if (group)
	{
		group->AddUIComponent(this);
	}
}

noa::UIComponent::~UIComponent()
{

}

void noa::UIComponent::Delete(UIComponent*& ptr)
{
	delete this;
	ptr = nullptr;
}

void noa::UIComponent::SetActive(bool active)
{
	this->active = active;
}

bool noa::UIComponent::GetActive()
{
	return active;
}

noa::Label::Label(UIGroup* group) :UIComponent(group)
{

}

noa::Label::~Label()
{

}

noa::Label* noa::Label::Create(UIGroup* group)
{
	Label* text = new Label(group);
	return text;
}

noa::Label& noa::Label::SetID(const std::string& id) 
{
	this->id = id;
	return *this;
}

noa::Label& noa::Label::SetAnchor(float x,float y) 
{
	this->anchor.x = x;
	this->anchor.y = y;
	return *this;
}

noa::Label& noa::Label::SetColor(uint32_t color) 
{
	this->color = color;
	return *this;
}

noa::Label& noa::Label::SetFontSize(uint32_t size)
{
	this->size = size;
	this->transform.size = renderer->GetLabelScale(this->text,this->size);
	return *this;
}

noa::Label& noa::Label::SetPosition(int x,int y) 
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::Label& noa::Label::SetText(const std::wstring& text) 
{
	this->text = text;
	this->transform.size = renderer->GetLabelScale(this->text, this->size);
	return *this;
}

noa::Label* noa::Label::Apply() 
{
	return this;
}


void noa::Label::Start()
{

}

void noa::Label::Update()
{

}

void noa::Label::Render() {
	//ÏÔÊ¾ÎÄ×Ö

	

	globalTransform.position.x = static_cast<int>(fatherTransform.position.x + transform.position.x - anchor.x * transform.size.x);
	globalTransform.position.y = static_cast<int>(fatherTransform.position.y + transform.position.y - anchor.y * transform.size.y);
	renderer->DrawString(
		text
		, globalTransform.position.x
		, globalTransform.position.y
		, color
		, size
	);
}



noa::Image::Image(UIGroup* group) :UIComponent(group)
{

}

noa::Image::~Image()
{
	
}

noa::Image* noa::Image::Create(UIGroup* group)
{
	Image* image = new Image(group);
	return image;
}

noa::Image& noa::Image::SetID(const std::string& id) 
{
	this->id = id;
	return *this;
}

noa::Image& noa::Image::SetPosition(int x,int y) 
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::Image& noa::Image::SetAnchor(float x,float y) 
{
	this->anchor.x = x;
	this->anchor.y = y;
	return *this;
}

noa::Image& noa::Image::SetSize(int x, int y)
{
	this->transform.size.x = x;
	this->transform.size.y = y;
	return *this;
}

noa::Image& noa::Image::SetStyle(noa::ImageStyle style)
{
	this->style = style;
	return *this;
}

noa::Image & noa::Image::SetSprite(Sprite* sprite)
{
	if (!sprite)
	{
		return *this;
	}
	this->sprite = sprite;
	if (spriteGPU)
	{
		//Èç¹ûÓĞÁËspriteGPU
		spriteGPU->Update(sprite);
		spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
		return *this;
	}
	spriteGPU = SpriteGPU::Create(sprite);
	spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
	return *this;
}

noa::Image& noa::Image::SetColor(uint32_t color) 
{
	this->color = color;
	return *this;
}

noa::Image* noa::Image::Apply() {
	return this;
}

void noa::Image::Start()
{

}

void noa::Image::Update()
{

	

}

void noa::Image::Render() {
	if (sprite == nullptr)
	{
		return;
	}

	globalTransform.position.x = static_cast<float>(fatherTransform.position.x + transform.position.x - anchor.x * transform.size.x);
	globalTransform.position.y = static_cast<float>(fatherTransform.position.y + transform.position.y - anchor.y * transform.size.y);

	switch (style)
	{
	case noa::ImageStyle::COVER:
		spriteGPU->DrawSprite(
			0
			, 0
			, static_cast<float>(noa::Screen::width)
			, static_cast<float>(noa::Screen::height)
			, color
			, isFilpX
			, 0.0f
		);
		break;
	default:
		spriteGPU->DrawSprite(
			globalTransform.position.x
			, globalTransform.position.y
			, static_cast<float>(transform.size.x)
			, static_cast<float>(transform.size.y)
			, color
			, isFilpX
			, 0.0f
		);
		break;
	}
}


noa::Button::Button(UIGroup* group) :UIComponent(group)
{
	image = Image::Create(group);
	label = Label::Create(group);
	
	label->anchor = { 0.5f,0.5f };

	this->SetSize(80, 40);
	image->SetSprite(&sprite);
}

noa::Button::~Button()
{

}

noa::Button* noa::Button::Create(UIGroup* group)
{
	Button* button = new Button(group);
	return button;
}

void noa::Button::SwapState()
{

	//buttonµÄ×´Ì¬×ª»»º¯Êı

	if (!active)
	{
		return;
	}

	const Vector<double>& mousePos = Input::GetMousePosition();

	const float mousePosX = (static_cast<float>(mousePos.x));
	const float mousePosY = (static_cast<float>(mousePos.y));

	const int posX = static_cast<int>(transform.position.x + fatherTransform.position.x - anchor.x * transform.size.x);
	const int posY = static_cast<int>(transform.position.y + fatherTransform.position.y - anchor.y * transform.size.y);

	//Í¨³£×´Ì¬
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	isClickReady = false;
	if (mousePosX > posX && mousePosX<posX + transform.size.x
		&& mousePosY>posY && mousePosY < posY + transform.size.y
		)
	{
		isSelect = true;//½øÈëµ½±»¸ßÁÁ×´Ì¬
		
		if (!selectEventFlag) 
		{
			//¸ßÁÁÊÂ¼ş
			this->selectedEvent.Invoke();
			selectEventFlag = true;
		}

		if (Input::GetMouseKeyHold(MouseButton::LEFT_BUTTON)) 
		{
			//½øÈëµã»÷×´Ì¬
			isClickReady = true;
		}
		else if (Input::GetMouseKeyUp(MouseButton::LEFT_BUTTON))
		{
			this->clickEvent.Invoke();
		}
		

	}
	else {
		isSelect = false;
		selectEventFlag = false;
	}

	if (!isClickReady) 
	{
		if (isSelect)
		{
			currentColor = heightLightColor;
			currentTextColor = textHeightLightColor;
			currentScale = heightLightScale;
		}
		else {
			currentColor = normalColor;
			currentTextColor = textNormalColor;
			currentScale = normalScale;
		}
	}
	else {
		currentColor = clickColor;
		currentTextColor = textClickColor;
		currentScale = clickScale;
	}

}

void noa::Button::Start()
{
	
}

void noa::Button::Update()
{
	this->SwapState();
}

void noa::Button::Render() {

	const float lerpSpeed = 450.0f;
	currentSize.x = noa::Math::LinearLerp(
		currentSize.x
		,transform.size.x * currentScale
		, lerpSpeed
	);
	currentSize.y = noa::Math::LinearLerp(
		currentSize.y
		,transform.size.y * currentScale
		, lerpSpeed
	);

	this->label->SetFontSize(static_cast<uint32_t>(fontSize * currentScale));

	image->anchor = anchor;
	image->transform.position = transform.position;
	image->transform.size.x = static_cast<int>(currentSize.x);
	image->transform.size.y = static_cast<int>(currentSize.y);
	image->color = currentColor;

	label->anchor = anchor;
	label->color = currentTextColor;
	label->transform.position.x = 
		static_cast<int>(transform.position.x 
			+ transform.size.x*labelOffset.x);
	label->transform.position.y = 
		static_cast<int>(transform.position.y 
			+ transform.size.y*labelOffset.y);
}

noa::Button& noa::Button::Clone(Button* button) {
	return this->SetNormalColor	(button->normalColor)
		.SetHeightLightColor		(button->heightLightColor)
		.SetClickColor			(button->clickColor)
		.SetSize					(button->transform.size.x,
									button->transform.size.y)
		.SetRadius				(button->radius)
		.SetTextNormalColor		(button->textNormalColor)
		.SetTextHeightLightColor	(button->textHeightLightColor)
		.SetTextClickColor		(button->textClickColor)
		.SetFontSize				(button->fontSize)
		.SetNormalScale			(button->normalScale)
		.SetHeightLightScale		(button->heightLightScale)
		.SetAnchor				(button->anchor.x,button->anchor.y)
		.SetTextOffset(button->labelOffset.x,button->labelOffset.y)
		.SetClickScale			(button->clickScale);
}

noa::Button& noa::Button::SetID(const std::string& id) 
{
	this->id = id;
	return *this;
}

noa::Button& noa::Button::SetNormalColor(uint32_t color) 
{
	this->normalColor = color;
	return *this;
}

noa::Button& noa::Button::SetHeightLightColor(uint32_t color)
{
	this->heightLightColor = color;
	return *this;
}

noa::Button& noa::Button::SetClickColor(uint32_t color) 
{
	this->clickColor = color;
	return *this;
}

noa::Button& noa::Button::SetPosition(int x,int y) 
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::Button& noa::Button::SetAnchor(float x,float y) 
{
	anchor.x = x;
	anchor.y = y;
	return *this;
}

noa::Button& noa::Button::SetSize(int w,int h) 
{
	this->transform.size.x = w;
	this->transform.size.y = h;

	this->currentSize.x = static_cast<float>(transform.size.x);
	this->currentSize.y = static_cast<float>(transform.size.y);

	SpriteFile spriteFile;
	spriteFile.width = w;
	spriteFile.height = h;
	spriteFile.images.resize(spriteFile.width * spriteFile.height, noa::RGBA(255, 255, 255, 255));
	
	this->sprite.size.x = w;
	this->sprite.size.y = h;
	this->sprite.UpdateImage(spriteFile);

	return *this;
}

noa::Button& noa::Button::SetRadius(int value)
{
	//ËÄ¸ö½ÇµÄ°ë¾¶

	if (value<=0) 
	{
		SpriteFile spriteFile;
		spriteFile.width = transform.size.x;
		spriteFile.height = transform.size.y;
		spriteFile.images.resize(spriteFile.width * spriteFile.height, noa::RGBA(255, 255, 255, 255));

		this->sprite.size.x = transform.size.x;
		this->sprite.size.y = transform.size.y;
		this->sprite.UpdateImage(spriteFile);

		radius = 0;
		return *this;
	}

	const int maxValue = (sprite.w < sprite.h) ? (sprite.w/2) : (sprite.h/2);
	radius = value;
	if (radius> maxValue)
	{
		radius = maxValue;
	}

	const int x1 = radius;
	const int x2 = sprite.w - radius;
	const int y1 = radius;
	const int y2 = sprite.h - radius;

	const int sqrRadius = (radius) * (radius);

	//×óÉÏ½Ç
	for (int x = 0;x<radius;x++) 
	{
		for (int y = 0;y<radius;y++) 
		{
			const int deltaX = x - x1;
			const int deltaY = y - y1;

			if (deltaX*deltaX + deltaY*deltaY > sqrRadius) {
				sprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	//×óÏÂ½Ç
	for (int x = 0; x < radius; x++)
	{
		for (int y = y2; y < sprite.h; y++)
		{
			const int deltaX = x - x1;
			const int deltaY = y - y2;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) {
				sprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	//ÓÒÏÂ½Ç
	for (int x = x2; x < sprite.w; x++)
	{
		for (int y = y2; y < sprite.h; y++)
		{
			const int deltaX = x - x2;
			const int deltaY = y - y2;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) {
				sprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
				
			}
		}
	}

	//ÓÒÉÏ½Ç
	for (int x = x2; x < sprite.w; x++)
	{
		for (int y = 0; y < radius; y++)
		{
			const int deltaX = x - x2;
			const int deltaY = y - y1;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) 
			{
				
				sprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	image->SetSprite(&sprite);

	return *this;
}

noa::Button& noa::Button::SetTextOffset(float x,float y) 
{
	labelOffset.x = x;
	labelOffset.y = y;
	return *this;
}

noa::Button& noa::Button::SetText(const std::wstring& text)
{
	this->label->SetText(text);
	return *this;
}

noa::Button& noa::Button::SetTextNormalColor(uint32_t color) 
{
	this->textNormalColor = color;
	return *this;
}

noa::Button& noa::Button::SetTextHeightLightColor(uint32_t color)
{
	this->textHeightLightColor = color;
	return *this;
}

noa::Button& noa::Button::SetTextClickColor(uint32_t color)
{
	this->textClickColor = color;
	return *this;
}

noa::Button& noa::Button::SetFontSize(uint32_t size) 
{
	this->fontSize = size;
	return *this;
}

noa::Button& noa::Button::SetNormalScale(float value) 
{
	this->normalScale = value;
	return *this;
}

noa::Button& noa::Button::SetHeightLightScale(float value)
{
	this->heightLightScale = value;
	return *this;
}

noa::Button& noa::Button::SetClickScale(float value)
{
	this->clickScale = value;
	return *this;
}

noa::Button& noa::Button::AddSelectedCallback(
	std::function<void()> func
)
{
	this->selectedEvent += func;
	return *this;
}


noa::Button& noa::Button::AddClickCallback(
	std::function<void()> func
)
{
	this->clickEvent += func;
	return *this;
}

noa::Button* noa::Button::Apply() {
	return this;
}


