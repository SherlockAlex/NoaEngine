#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Resource.h"
#include "Sprite.h"
#include "FontAsset.h"
#include "Font.h"
#include "Graphic.h"
#include "Debug.h"

noa::SpriteFile CreateSpriteFromBitmap(FT_Bitmap* bitmap)
{
	noa::SpriteFile sprite;

	sprite.width = bitmap->width;
	sprite.height = bitmap->rows;
	sprite.images.resize(sprite.width * sprite.height, noa::RGBA(0, 0, 0, 0));

	if (bitmap->width == 0 || bitmap->rows == 0)
	{
		sprite.width = 0;
		sprite.height = 0;
		return sprite;
	}

	for (uint32_t y = 0; y < bitmap->rows; y++) {
		for (uint32_t x = 0; x < bitmap->width; x++) {

			uint8_t pixelValue = bitmap->buffer[y * (bitmap->width) + x];
			uint32_t pixelColor = noa::RGBA(255, 255, 255, pixelValue);

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

	FT_Select_Charmap(face, ft_encoding_unicode);
	FT_Set_Pixel_Sizes(face, 0, size);


	std::wstring chineseChar;
	chineseChar.append(L"һ�Ҷ�ʮ�����߲�����˾ż��������˵������ڸɿ�ʿ�����Ŵ��´���������С�ڽ�ɽǧ���ڸ�");
	chineseChar.append(L"�׾÷���Ϧ��ô��������֮ʬ����������ҲŮ����ϰ�������������������Ԫר������ľ��֧����");
	chineseChar.append(L"̫Ȯ��������ƥ�������ͱȻ�����ֹ�����иԱ���ˮ����ţ��ë��������ʲƬ�ͻ�����Խ���צ��");
	chineseChar.append(L"�鸸�ӽ��׷ַ�����������Ƿ�絤���ڷﹴ��������Ϊ���䶩�ƻ����ĳ�����Ϳ׶Ӱ�������Ȱ˫");
	chineseChar.append(L"�����ʾĩδ���������˰ǹ���ȥ�����Žڱ����ɱ�������ʯ����ƽ����������ռҵ��˧���ҵ�");
	chineseChar.append(L"ĿҶ���궣�������ʷֻ���ֵ����߶̾����ʧ�����̴������ǰ�������Ϻ�������˦ӡ�־��");
	chineseChar.append(L"�᷸�⴦���������������������֭��ͷ����Ѩ����д����ѵ����Ѷ����˾���������ū����Ƥ��");
	chineseChar.append(L"����ʥ��̨ì��ĸ��˿ʽ�̶����¼��ۿ�����ִ������ɨ���ﳡ����â��֥���ӻ�Ȩ������Э��ѹ");
	chineseChar.append(L"�����аٴ��ҳ�����Ҵ������ɼй�а������������ʦ�����ӹ⵱�����ų�����ͬ������������");
	chineseChar.append(L"�������������������ȶ�����Ǩ��ΰ��ƹ��������ŷ��Ӽ����˼۷ݻ����»�α��Ѫ���ƺ�����");
	chineseChar.append(L"ȫ��ɱ��������үɡ��������ΣѮּ����������ɫ׳���ׯ�������뽻���²����������ʴ��򲢹�");
	chineseChar.append(L"�׵��ݺ��۽�����æ������լ�ְ���������ũ�����Ѱ��Ѹ�������������ս��������績������Ϸ");
	chineseChar.append(L"��ۻ�����˼�Լ�ͳ�Ѳ��Ū���ν�����ԶΥ�˷���̳�����ž�������ַ�߳��ӹ�������ץ����Т");
	chineseChar.append(L"����Ͷ�ؿ��ӷ�������־Ť�����ѱ�ȴ��ѿ���۷ҲԷ���«�Ϳ��ո˸ܶŲĴ��Ӽ��������������");
	chineseChar.append(L"��ҽ�����񻹼��������ẵ����ʱ���������԰��Χѽ��������������Ա���Դ��ذɺ����ʲ���");
	chineseChar.append(L"����������ͺ��˽ÿ������ε���������Ӷ����סλ�����������۷���ϣ�����׺��ڲ�ζǳ���");
	chineseChar.append(L"����̽�ɾ���ѵ�ӭ����ϵ�Զ�״Ķ��������Ӧ����������ұ���м�������ӵ���ɳ���ַ���û��");
	chineseChar.append(L"�����ǿ����κ��ξ�������֤����������ʶ���������鼴����β�پָ��żɼ�½�����踽������");
	chineseChar.append(L"Ŭ�̾�������ɴ�ɸٲ��ݷ�ֽ�Ʒ�¿Ŧ���滷�������ֱ��Ĩ£�μ�̹Ѻ��������߶���ӵ�־�");
	chineseChar.append(L"�Ʊ���������������������̧��ȡ����ïƻ��Ӣ��ֱ�Ѿ�é��֦����������ǹ��������ɥ�����´�");
	chineseChar.append(L"����������ޱ����̬ŷ¢�޺���תն��������ϳ�Щ��²���������߹�ζ�����������װ����");
	chineseChar.append(L"�Ҹ�����ӽ�ذ����������뿭�ܷ���ͼ����֪������ԹθѺͼ�ί���̹�ʹ����ֶ���ƾ�������");
	chineseChar.append(L"���������������˾�����������ֲ�����̰��ƶ����֫������ɷʷ�в�ܻ����ú��������α��Ǳ�");
	chineseChar.append(L"�����ҹ���׼����Ͼ�ä�ſ���բ��֣ȯ����������¯ĭǳ��й��մ���Ͳ�����עкӾ��в�");
	chineseChar.append(L"�����β���������ѧ���ڶ�������ٿ���ʵ����ʫ�緿�ϳ����ӻ���ѯ���꽨��¼���ӽ�ˢ���ҳ�");
	chineseChar.append(L"�Ϲ��½����ùý���ʼ�ݲμ�������ϸʻ֯��פ���ܾ����ഺ���䲣���͹ҷ���������������");
	chineseChar.append(L"��ͦ��˩ʰ��ָ������ƴ�ڰ���Ųĳ���������ݼ���ã���ٹʺ���ҩ��ݱ���������������");
	chineseChar.append(L"��Ҫ��������ש�����������ˣǣ������ѻ�Ա�ս��������ʡ��������գ������ðӳ����ηſθ��");
	chineseChar.append(L"���Ϻ��˼����Ʒ��������ҧ����̿Ͽ�������ǳ��Ӹ�Կ��ж�װݿ�������ѡ�������������");
	chineseChar.append(L"���Ͷα�����˳�ޱ�������׷��Ż�Ȫ����׷���ܴ��ɺ�������ʳ�赨ʤ����������ʨ��������");
	chineseChar.append(L"óԹ����ʴ�ȱ��佫����ͤ���ȼ�ͥ�����߰���������ʩ�ŷ������������������ǰ��������ը��");
	chineseChar.append(L"�����������Ƕ���ϴ����ǢȾ�����޻�Ũ����ǡ�պ޾پ����ҹ���ͻ���Կ͹���������ף��");
	chineseChar.append(L"��˵�п��˼�����Ѷ�ü������Ժ����������ŭ�ܺ�ӯ�µ����ݰ��޽��ƽ�����������ͳ������");
	chineseChar.append(L"̩����ز���յ�����Բ����ظ�����������׽���������ż����ȿֺ����ܵ�����Ī�ɻ������");
	chineseChar.append(L"���ͩ�����Ҹ�У�����������ٶ�������贽�Ĵ���ԭ��������˽ν϶ٱ��²����Ǽ����ɹ��");
	chineseChar.append(L"��Ѽ���������ڿ޶��������շ�Բ����Ǯǯ������Ǧȱ��������˵г�������ȳ���͸��Ц��ծ��");
	chineseChar.append(L"ֵ���㵹�Ⱦ㳫�򸩱��뽡���乪Ϣͽ�콢�հ㺽;�õ������̴�֬�ظ��ེ�����Ƿ����������");
	chineseChar.append(L"��˥��ϯ׼����֢������ƣЧ��������վ�ʾ�������������ƿȭ������濾�淳�����̵������Ծ�");
	chineseChar.append(L"�����ƺ�Ϳԡ�������˽�����ӿ���Ļ��ú���������խ���װ���������������۱����˭��ԩ��");
	chineseChar.append(L"̸�����չ��м��������������ͨ����Ԥɣ��������������������ڽ��ŵ��������ڽ�������ӿ�");
	chineseChar.append(L"̽�ݾ�ְ�����ջ����ܾ����Ѿ�Ƽ��Ӫе����÷������Ͱ�ȸ�Ʊ��ˬ��Ϯʢѩ������ȸ�ó��׳���");
	chineseChar.append(L"������Ұ�����ľ�Ծ�����۳���Ψ��ո��Ȧͭ�����������Ʊ����ѷ����������Ƴ�ż͵����ͣƫ��");
	chineseChar.append(L"�����̴�б�и�Ϥ������Ų������󹻲�����è���ڹݴռ����������ȿ�ӹ¹���¾�������������");
	chineseChar.append(L"��ճ�����ϼ���������������������Һ������������ϧ�ѵ����辪�ҹ߿ܼ���Ҥ��ı�ѻ��մ�����");
	chineseChar.append(L"���浰¡������������������ά������ٰ�������Խ��������̲���ϲ�徾����Ԯ�ø�§����");
	chineseChar.append(L"��˹������ɢ������Ͼ����䳯��������ֲɭ�ν��ù������ػݻ�Ƴ���Ӳȷ��ֳ�������ű���");
	chineseChar.append(L"�ϻԳ��������������羧���������������������Ѻ�ι�����ñ���������������������������");
	chineseChar.append(L"̺��ʣ�Գ�ϡ˰�������ɸͲ����ݰ����Ʊ����������½ֳ���ѭͧ�淬������Ƣǻ³����Ȼ��װ");
	chineseChar.append(L"����ʹͯ�������շ��������ۺ���ʪ�¿ʻ�������̸ȷ߻Ŷ����修������Ѵ���ԣ��ȹлҥ");
	chineseChar.append(L"ǫ����ǿ�����϶��ɩ�Ƕл���ƭԵ�������������İ�Я��ҡ����̯����ȵ��ĹĻ����������");
	chineseChar.append(L"�����뻱��¥������а���������µ�������䶽�����˯�Ǳ���ů��Ъ���տ�����·��ǲ���ɤ");
	chineseChar.append(L"�����ִ����സ�����Ⱛ�ǳ���ǩ��پ����ɵ���΢��ң���ȸ����ȴ��⽴̵��������������");
	chineseChar.append(L"�ܴ�ú����ĮԴ������Ϫ�������̲����������Ⱥ������Ӽ޵����������ǽƲ�δݽ��ľ�ժˤ��");
	chineseChar.append(L"��Ľĺ��ģ���ե����������Ը����ѿ�������Ӭ֩׬�¶�����������ű���òĤ������������");
	chineseChar.append(L"�ú����ڸ������߶��쾫ǸϨ����Ư������©��կ���������۴��ܵ�������˺��Ȥ�˳Ų�ײ������");
	chineseChar.append(L"Ь���ߺ��ӣ��Ʈ������ù���ⱩϹӰ��̤���ٵ�����ī�򿿵��������ƪ����Ƨ����ϥ����Ħ");
	chineseChar.append(L"�������Ǳ������ο��������н�����������Ѳ���������Ĭ�������������ĥ������Ǹ�ȼ�輤��");
	chineseChar.append(L"�ڱܽɴ����ϲ�˪ϼ�Ƶ����뷱��Ӯ�㿷�������޸�������ӥ���ʶײ��걬����ҫ����¼�ħ��");
	chineseChar.append(L"����¶�ҹ�~����@#��%����&*��������++-=��|��{}������������������/��*");

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
	if (this->fonts.count(c) <= 0)
	{
		return nullptr;
	}
	return fonts[c];
}

noa::Font* noa::FontAsset::operator[](wchar_t c) {
	return GetFont(c);
}