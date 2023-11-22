#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include "Renderer.h"
#include "Debug.h"
#include "NoaGUI.h"
#include "Sprite.h"
#include "InputSystem.h"
#include "Screen.h"
#include "AudioSystem.h"

extern std::shared_ptr<noa::Renderer> noa::renderer;

std::vector<noa::UIDocument*> noa::UIHub::documents;

noa::SpriteFile CreateSpriteFromBitmap(FT_Bitmap* bitmap)
{
	noa::SpriteFile sprite;

	sprite.width = bitmap->width;
	sprite.height = bitmap->rows;
	sprite.images.resize(sprite.width * sprite.height, noa::RGBA(0,0,0,0));

	if (bitmap->width == 0 || bitmap->rows == 0) 
	{
		sprite.width = 0;
		sprite.height = 0;
		return sprite;
	}

	for (uint32_t y = 0; y < bitmap->rows; y++) {
		for (uint32_t x = 0; x < bitmap->width; x++) {

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
		chineseChar.append(L"一乙二十丁厂七卜人入八九几儿了力乃刀又三于干亏士工土才寸下大丈与万上小口巾山千乞川亿个");
		chineseChar.append(L"勺久凡及夕丸么广亡门义之尸弓己已子卫也女飞刃习叉马乡丰王井开夫天无元专云扎艺木五支厅不");
		chineseChar.append(L"太犬区历尤友匹车巨牙屯比互切瓦止少日中冈贝内水见午牛手毛气升长仁什片仆化仇币仍仅斤爪反");
		chineseChar.append(L"介父从今凶分乏公仓月氏勿欠风丹匀乌凤勾文六方火为斗忆订计户认心尺引丑巴孔队办以允予劝双");
		chineseChar.append(L"书幻玉刊示末未击打巧正扑扒功扔去甘世古节本术可丙左厉右石布龙平灭轧东卡北占业旧帅归且旦");
		chineseChar.append(L"目叶甲申叮电号田由史只央兄叼叫另叨叹四生失禾丘付仗代仙们仪白仔他斥瓜乎丛令用甩印乐句匆");
		chineseChar.append(L"册犯外处冬鸟务包饥主市立闪兰半汁汇头汉宁穴它讨写让礼训必议讯记永司尼民出辽奶奴加召皮边");
		chineseChar.append(L"发孕圣对台矛纠母幼丝式刑动扛寺吉扣考托老执巩圾扩扫地扬场耳共芒亚芝朽朴机权过臣再协西压");
		chineseChar.append(L"厌在有百存而页匠夸夺灰达列死成夹轨邪划迈毕至此贞师尘尖劣光当早吐吓虫曲团同吊吃因吸吗屿");
		chineseChar.append(L"帆岁回岂刚则肉网年朱先丢舌竹迁乔伟传乒乓休伍伏优伐延件任伤价份华仰仿伙伪自血向似后行舟");
		chineseChar.append(L"全会杀合兆企众爷伞创肌朵杂危旬旨负各名多争色壮冲冰庄庆亦刘齐交次衣产决充妄闭问闯羊并关");
		chineseChar.append(L"米灯州汗污江池汤忙兴宇守宅字安讲军许论农讽设访寻那迅尽导异孙阵阳收阶阴防奸如妇好她妈戏");
		chineseChar.append(L"羽观欢买红纤级约纪驰巡寿弄麦形进戒吞远违运扶抚坛技坏扰拒找批扯址走抄坝贡攻赤折抓扮抢孝");
		chineseChar.append(L"均抛投坟抗坑坊抖护壳志扭块声把报却劫芽花芹芬苍芳严芦劳克苏杆杠杜材村杏极李杨求更束豆两");
		chineseChar.append(L"丽医辰励否还歼来连步坚旱盯呈时吴助县里呆园旷围呀吨足邮男困吵串员听吩吹呜吧吼别岗帐财针");
		chineseChar.append(L"钉告我乱利秃秀私每兵估体何但伸作伯伶佣低你住位伴身皂佛近彻役返余希坐谷妥含邻岔肝肚肠龟");
		chineseChar.append(L"免狂犹角删条卵岛迎饭饮系言冻状亩况床库疗应冷这序辛弃冶忘闲间闷判灶灿弟汪沙汽沃泛沟没沈");
		chineseChar.append(L"沉怀忧快完宋宏牢究穷灾良证启评补初社识诉诊词译君灵即层尿尾迟局改张忌际陆阿陈阻附妙妖妨");
		chineseChar.append(L"努忍劲鸡驱纯纱纳纲驳纵纷纸纹纺驴纽奉玩环武青责现表规抹拢拔拣担坦押抽拐拖拍者顶拆拥抵拘");
		chineseChar.append(L"势抱垃拉拦拌幸招坡披拨择抬其取苦若茂苹苗英范直茄茎茅林枝杯柜析板松枪构杰述枕丧或卧事刺");
		chineseChar.append(L"枣雨卖矿码厕奔奇奋态欧垄妻轰顷转斩轮软到非叔肯齿些虎虏肾贤尚旺具果味昆国昌畅明易昂典固");
		chineseChar.append(L"忠咐呼鸣咏呢岸岩帖罗帜岭凯败贩购图钓制知垂牧物乖刮秆和季委佳侍供使例版侄侦侧凭侨佩货依");
		chineseChar.append(L"的迫质欣征往爬彼径所舍金命斧爸采受乳贪念贫肤肺肢肿胀朋股肥服胁周昏鱼兔狐忽狗备饰饱饲变");
		chineseChar.append(L"京享店夜庙府底剂郊废净盲放刻育闸闹郑券卷单炒炊炕炎炉沫浅法泄河沾泪油泊沿泡注泻泳泥沸波");
		chineseChar.append(L"泼泽治怖性怕怜怪学宝宗定宜审宙官空帘实试郎诗肩房诚衬衫视话诞询该详建肃录隶居届刷屈弦承");
		chineseChar.append(L"孟孤陕降限妹姑姐姓始驾参艰线练组细驶织终驻驼绍经贯奏春帮珍玻毒型挂封持项垮挎城挠政赴赵");
		chineseChar.append(L"挡挺括拴拾挑指垫挣挤拼挖按挥挪某甚革荐巷带草茧茶荒茫荡荣故胡南药标枯柄栋相查柏柳柱柿栏");
		chineseChar.append(L"树要咸威歪研砖厘厚砌砍面耐耍牵残殃轻鸦皆背战点临览竖省削尝是盼眨哄显哑冒映星昨畏趴胃贵");
		chineseChar.append(L"界虹虾蚁思蚂虽品咽骂哗咱响哈咬咳哪炭峡罚贱贴骨钞钟钢钥钩卸缸拜看矩怎牲选适秒香种秋科重");
		chineseChar.append(L"复竿段便俩贷顺修保促侮俭俗俘信皇泉鬼侵追俊盾待律很须叙剑逃食盆胆胜胞胖脉勉狭狮独狡狱狠");
		chineseChar.append(L"贸怨急饶蚀饺饼弯将奖哀亭亮度迹庭疮疯疫疤姿亲音帝施闻阀阁差养美姜叛送类迷前首逆总炼炸炮");
		chineseChar.append(L"烂剃洁洪洒浇浊洞测洗活派洽染济洋洲浑浓津恒恢恰恼恨举觉宣室宫宪突穿窃客冠语扁袄祖神祝误");
		chineseChar.append(L"诱说诵垦退既屋昼费陡眉孩除险院娃姥姨姻娇怒架贺盈勇怠柔垒绑绒结绕骄绘给络骆绝绞统耕耗艳");
		chineseChar.append(L"泰珠班素蚕顽盏匪捞栽捕振载赶起盐捎捏埋捉捆捐损都哲逝捡换挽热恐壶挨耻耽恭莲莫荷获晋恶真");
		chineseChar.append(L"框桂档桐株桥桃格校核样根索哥速逗栗配翅辱唇夏础破原套逐烈殊顾轿较顿毙致柴桌虑监紧党晒眠");
		chineseChar.append(L"晓鸭晃晌晕蚊哨哭恩唤啊唉罢峰圆贼贿钱钳钻铁铃铅缺氧特牺造乘敌秤租积秧秩称秘透笔笑笋债借");
		chineseChar.append(L"值倚倾倒倘俱倡候俯倍倦健臭射躬息徒徐舰舱般航途拿爹爱颂翁脆脂胸胳脏胶脑狸狼逢留皱饿恋桨");
		chineseChar.append(L"浆衰高席准座脊症病疾疼疲效离唐资凉站剖竞部旁旅畜阅羞瓶拳粉料益兼烤烘烦烧烛烟递涛浙涝酒");
		chineseChar.append(L"涉消浩海涂浴浮流润浪浸涨烫涌悟悄悔悦害宽家宵宴宾窄容宰案请朗诸读扇袜袖袍被祥课谁调冤谅");
		chineseChar.append(L"谈谊剥恳展剧屑弱陵陶陷陪娱娘通能难预桑绢绣验继球理捧堵描域掩捷排掉堆推掀授教掏掠培接控");
		chineseChar.append(L"探据掘职基著勒黄萌萝菌菜萄菊萍菠营械梦梢梅检梳梯桶救副票戚爽聋袭盛雪辅辆虚雀堂常匙晨睁");
		chineseChar.append(L"眯眼悬野啦晚啄距跃略蛇累唱患唯崖崭崇圈铜铲银甜梨犁移笨笼笛符第敏做袋悠偿偶偷您售停偏假");
		chineseChar.append(L"得衔盘船斜盒鸽悉欲彩领脚脖脸脱象够猜猪猎猫猛馅馆凑减毫麻痒痕廊康庸鹿盗章竟商族旋望率着");
		chineseChar.append(L"盖粘粗粒断剪兽清添淋淹渠渐混渔淘液淡深婆梁渗情惜惭悼惧惕惊惨惯寇寄宿窑密谋谎祸谜逮敢屠");
		chineseChar.append(L"弹随蛋隆隐婚婶颈绩绪续骑绳维绵绸绿琴斑替款堪搭塔越趁趋超提堤博揭喜插揪搜煮援裁搁搂搅握");
		chineseChar.append(L"揉斯期欺联散惹葬葛董葡敬葱落朝辜葵棒棋植森椅椒棵棍棉棚棕惠惑逼厨厦硬确雁殖裂雄暂雅辈悲");
		chineseChar.append(L"紫辉敞赏掌晴暑最量喷晶喇遇喊景践跌跑遗蛙蛛蜓喝喂喘喉幅帽赌赔黑铸铺链销锁锄锅锈锋锐短智");
		chineseChar.append(L"毯鹅剩稍程稀税筐等筑策筛筒答筋筝傲傅牌堡集焦傍储奥街惩御循艇舒番释禽腊脾腔鲁猾猴然馋装");
		chineseChar.append(L"蛮就痛童阔善羡普粪尊道曾焰港湖渣湿温渴滑湾渡游滋溉愤慌惰愧愉慨割寒富窜窝窗遍裕裤裙谢谣");
		chineseChar.append(L"谦属屡强粥疏隔隙絮嫂登缎缓编骗缘瑞魂肆摄摸填搏塌鼓摆携搬摇搞塘摊蒜勤鹊蓝墓幕蓬蓄蒙蒸献");
		chineseChar.append(L"禁楚想槐榆楼概赖酬感碍碑碎碰碗碌雷零雾雹输督龄鉴睛睡睬鄙愚暖盟歇暗照跨跳跪路跟遣蛾蜂嗓");
		chineseChar.append(L"置罪罩错锡锣锤锦键锯矮辞稠愁筹签简毁舅鼠催傻像躲微愈遥腰腥腹腾腿触解酱痰廉新韵意粮数煎");
		chineseChar.append(L"塑慈煤煌满漠源滤滥滔溪溜滚滨粱滩慎誉塞谨福群殿辟障嫌嫁叠缝缠静碧璃墙撇嘉摧截誓境摘摔聚");
		chineseChar.append(L"蔽慕暮蔑模榴榜榨歌遭酷酿酸磁愿需弊裳颗嗽蜻蜡蝇蜘赚锹锻舞稳算箩管僚鼻魄貌膜膊膀鲜疑馒裹");
		chineseChar.append(L"敲豪膏遮腐瘦辣竭端旗精歉熄熔漆漂漫滴演漏慢寨赛察蜜谱嫩翠熊凳骡缩慧撕撒趣趟撑播撞撤增聪");
		chineseChar.append(L"鞋蕉蔬横槽樱橡飘醋醉震霉瞒题暴瞎影踢踏踩踪蝶蝴嘱墨镇靠稻黎稿稼箱箭篇僵躺僻德艘膝膛熟摩");
		chineseChar.append(L"颜毅糊遵潜潮懂额慰劈操燕薯薪薄颠橘整融醒餐嘴蹄器赠默镜赞篮邀衡膨雕磨凝辨辩糖糕燃澡激懒");
		chineseChar.append(L"壁避缴戴擦鞠藏霜霞瞧蹈螺穗繁辫赢糟糠燥臂翼骤鞭覆蹦镰翻鹰警攀蹲颤瓣爆疆壤耀躁嚼嚷籍魔灌");
		chineseChar.append(L"蠢霸露囊罐~·！@#￥%……&*（）——++-=、|【{}】；：‘“，《。》/？*");

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

noa::UIDocument* noa::UIHub::GetDocumentByID(
	const std::string& id)
{
	for (auto& document:documents)
	{
		if (document&&document->id == id) 
		{
			return document;
		}
	}
	return nullptr;
}

void noa::UIHub::RemoveDocument(noa::UIDocument* document) {
	auto it = std::find(
		documents.begin()
		, documents.end()
		, document);
	if (it == documents.end()) 
	{
		return;
	}
	documents.erase(it);
}

noa::UIDocument::UIDocument() {
	noa::UIHub::documents.push_back(this);
}

noa::UIDocument::~UIDocument() 
{
	noa::UIHub::RemoveDocument(this);
	while (!containerStack.empty())
	{
		containerStack.pop();
	}
	for (auto & container: containerList)
	{
		container->Delete(container);
	}
}

void noa::UIDocument::AddUIContainer(UIContainer* container)
{
	if (container == nullptr)
	{
		return;
	}
	container->index = containerList.size();
	this->containerList.push_back(container);

}

void noa::UIDocument::SetDocumentID(const std::string& id)
{
	this->id = id;
}

void noa::UIDocument::Display(size_t index)
{
	if (index >= containerList.size()|| containerList[index] == nullptr)
	{
		return;
	}
	containerList[index]->SetVisiable(true);
	this->containerStack.push(containerList[index]);
}

void noa::UIDocument::Display(const std::string& id)
{
	UIContainer* container = this->GetElementByID<UIContainer>(id);
	if (!container)
	{
		return;
	}
	Display(container->GetContainerIndex());
}

void noa::UIDocument::Display(noa::UIContainer* container)
{
	if (!container)
	{
		return;
	}
	
	Display(container->GetContainerIndex());
}

void noa::UIDocument::Close() {
	if (containerStack.empty())
	{
		return;
	}
	containerStack.top()->SetVisiable(false);
	containerStack.pop();
}

void noa::UIDocument::UIDocumentStart() {
	
}

void noa::UIDocument::UIDocumentUpdate()
{
	if (containerStack.empty())
	{
		return;
	}

	//控制逻辑只控制栈顶
	if (!containerStack.empty()) 
	{
		containerStack.top()->Update();
	}
	
}

void noa::UIDocument::UIDocumentRender() {
	for (auto& container : containerList)
	{
		//越往前的越先绘制
		if (container && container->visiable)
		{
			container->Render();
		}
	}
}

noa::UIContainer::UIContainer(UIDocument* document)
{
	if (document)
	{
		document->AddUIContainer(this);
	}
	this->SetVisiable(false);
}

noa::UIContainer::UIContainer(UIContainer* father) 
{
	if (father)
	{
		father->AddUIContainer(this);
	}
	this->SetVisiable(false);
}

noa::UIContainer::~UIContainer() {

	//删除控件
	DestroyUIContainer();
}

noa::UIContainer* noa::UIContainer::Create(noa::UIDocument* document)
{
	if (document == nullptr) 
	{
		return nullptr;
	}

	UIContainer* container = new UIContainer(document);
	return container;

}

noa::UIContainer* noa::UIContainer::Create(noa::UIContainer* father) 
{
	if (father == nullptr)
	{
		return nullptr;
	}
	UIContainer* container = new UIContainer(father);
	return container;
}

void noa::UIContainer::Delete(UIContainer*& ptr)
{
	delete this;
	ptr = nullptr;
}

noa::UIContainer& noa::UIContainer::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::UIContainer& noa::UIContainer::SetPosition(int x,int y)
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::UIContainer& noa::UIContainer::SetGlobalPosition(int x,int y) 
{
	transform.position.x =
		x - fatherTransform.position.x;

	transform.position.y =
		y - fatherTransform.position.y;

	return *this;
}

noa::UIContainer& noa::UIContainer::SetVisiable(bool value) 
{
	this->visiable = value;
	return *this;
}

noa::UIContainer* noa::UIContainer::Apply() {
	return this;
}

void noa::UIContainer::AddUIComponent(UIComponent* component)
{
	if (component == nullptr)
	{
		return;
	}
	component->SetActiveInContainer(true);
	uiComponent.push_back(component);
}

void noa::UIContainer::AddUIContainer(UIContainer* container) 
{
	if (container == nullptr) 
	{
		return;
	}

	container->SetVisiable(true);
	subContainers.push_back(container);

}

size_t noa::UIContainer::GetContainerIndex() {
	return this->index;
}

void noa::UIContainer::Start() {
	for (auto& component : uiComponent)
	{
		if (component != nullptr)
		{
			component->Start();
		}
	}
}

void noa::UIContainer::Update()
{

	globalTransform.position = fatherTransform.position + transform.position;
	//显示背景
	
	if (!visiable) 
	{
		return;
	}

	for (auto& container:subContainers) 
	{
		if (container == nullptr||!container->visiable) 
		{
			continue;
		}
		container->fatherTransform = this->globalTransform;
		container->Update();
	}

	for (auto& component : uiComponent)
	{
		if (component == nullptr || !component->active)
		{
			continue;
		}
		component->fatherTransform = this->globalTransform;
		component->Update();
	}
}

void noa::UIContainer::Render() {

	if (!visiable)
	{
		return;
	}

	//显示背景
	for (auto& container : subContainers)
	{
		if (container == nullptr || !container->visiable)
		{
			continue;
		}
		container->fatherTransform = this->globalTransform;
		container->Render();
	}

	for (auto& component : uiComponent)
	{
		if (component == nullptr || !component->active)
		{
			continue;
		}
		component->fatherTransform = this->globalTransform;
		component->Render();
	}

}

void noa::UIContainer::DestroyUIContainer() {
	if (!this->uiComponent.empty())
	{
		//对component进行排序
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

	uiComponent.clear();

	if (!subContainers.empty())
	{
		std::sort(subContainers.begin(),subContainers.end());
		auto last = std::unique(subContainers.begin(), subContainers.end());
		subContainers.erase(last,subContainers.end());

		for (auto& child:subContainers) 
		{
			if (child) 
			{
				child->Delete(child);
			}
		}
	}
	subContainers.clear();

}

noa::UIDocumentActor::UIDocumentActor(Scene* scene) :Actor(scene), UIDocument()
{

}

noa::UIDocumentActor::~UIDocumentActor()
{
	
}

noa::UIDocumentActor* noa::UIDocumentActor::Create(Scene* scene)
{
	return noa::NObject<UIDocumentActor>::Create(scene);
}

void noa::UIDocumentActor::Start()
{
	UIDocument::UIDocumentStart();
}

void noa::UIDocumentActor::Update()
{
	UIDocument::UIDocumentUpdate();
}

void noa::UIDocumentActor::Render() {
	UIDocument::UIDocumentRender();
}

noa::UIDocumentActor& noa::UIDocumentActor::SetActorTag(
	const std::string& tag)
{
	this->tag = tag;
	return *this;
}

noa::UIDocumentActor& noa::UIDocumentActor::SetID(
	const std::string& id) 
{
	SetDocumentID(id);
	return *this;
}

noa::UIDocumentActor* noa::UIDocumentActor::Apply() {
	return this;
}

noa::UIDocumentComponent::UIDocumentComponent(Actor* actor) :ActorComponent(actor)
{

}

noa::UIDocumentComponent::~UIDocumentComponent() {
	
}

noa::UIDocumentComponent* noa::UIDocumentComponent::Create(Actor * actor)
{
	return noa::NObject<UIDocumentComponent>::Create(actor);
}

noa::UIDocumentComponent& noa::UIDocumentComponent::SetID(
	const std::string& id)
{
	noa::UIDocument::SetDocumentID(id);
	return *this;
}

noa::UIDocumentComponent* noa::UIDocumentComponent::Apply() {
	return this;
}

void noa::UIDocumentComponent::Start() {
	UIDocument::UIDocumentStart();
}

void noa::UIDocumentComponent::Update() {
	UIDocument::UIDocumentUpdate();
}

void noa::UIDocumentComponent::Render() {
	UIDocument::UIDocumentRender();
}

noa::UIComponent::UIComponent(noa::UIContainer* group)
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

void noa::UIComponent::SetGlobalPosition(int x,int y) {
	//计算值，然后换算成position
	//globalTransform.position.x 
	// = (fatherTransform.position.x 
	// + transform.position.x 
	// - anchor.x * transform.size.x);

	transform.position.x = 
		x + anchor.x * transform.size.x 
		- fatherTransform.position.x;

	transform.position.y =
		y + anchor.y * transform.size.y
		- fatherTransform.position.y;

}

void noa::UIComponent::SetActiveInContainer(bool active)
{
	this->active = active;
}

bool noa::UIComponent::GetActive()
{
	return active;
}

noa::Label::Label(UIContainer* group) :UIComponent(group)
{

}

noa::Label::~Label()
{

}

noa::Label* noa::Label::Create(UIContainer* group)
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

noa::Label& noa::Label::SetActive(bool value) 
{
	UIComponent::SetActiveInContainer(value);
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
	//显示文字

	

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



noa::Image::Image(UIContainer* group) :UIComponent(group)
{
	Sprite noneSprite;
	noneSprite.w = 320;
	noneSprite.h = 320;
	noneSprite.size = { 320,320 };
	noneSprite.ResizeAndFull(320, 320, noa::RGBA(255, 255, 255, 255));

	spriteGPU = SpriteGPU::Create(&noneSprite);
	spriteGPU->SetLayer(InstanceLayer::UI_LAYER);
}

noa::Image::~Image()
{
	
}

noa::Image* noa::Image::Create(UIContainer* group)
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

noa::Image& noa::Image::SetActive(bool value)
{
	UIComponent::SetActiveInContainer(value);
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
	//如果有了spriteGPU
	spriteGPU->Update(sprite);
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

	globalTransform.position.x = static_cast<int>(fatherTransform.position.x + transform.position.x - anchor.x * transform.size.x);
	globalTransform.position.y = static_cast<int>(fatherTransform.position.y + transform.position.y - anchor.y * transform.size.y);

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
			static_cast<float>(globalTransform.position.x)
			, static_cast<float>(globalTransform.position.y)
			, static_cast<float>(transform.size.x)
			, static_cast<float>(transform.size.y)
			, color
			, isFilpX
			, 0.0f
		);
		break;
	}
}


noa::Button::Button(UIContainer* group) :UIComponent(group)
{
	image = Image::Create(group);
	label = Label::Create(group);
	
	label->anchor = { 0.5f,0.5f };

	SetFontSize(20);
	SetPosition(0, 0);
	SetSize(240, 60);
	SetRadius(50);
	SetNormalColor(noa::RGBA(255, 255, 255, 255));
	SetHeightLightColor(noa::RGBA(255, 0, 0, 255));
	SetAnchor(0.5f, 0.5f);
	SetTextOffset(0.0f, 0.0f);
	SetTextNormalColor(noa::BLACK);
	SetTextHeightLightColor(noa::WHITE);
	SetTextClickColor(noa::BLACK);
	SetNormalScale(1.0f);
	SetHeightLightScale(1.1f);
	SetClickScale(0.9f);

	image->SetSprite(&sprite);
}

noa::Button::~Button()
{

}

noa::Button* noa::Button::Create(UIContainer* group)
{
	Button* button = new Button(group);
	return button;
}

void noa::Button::SwapState()
{

	//button的状态转换函数

	if (!active)
	{
		return;
	}

	const Vector<double>& mousePos = Input::GetMousePosition();

	const float mousePosX = (static_cast<float>(mousePos.x));
	const float mousePosY = (static_cast<float>(mousePos.y));

	const int posX = static_cast<int>(transform.position.x + fatherTransform.position.x - anchor.x * transform.size.x);
	const int posY = static_cast<int>(transform.position.y + fatherTransform.position.y - anchor.y * transform.size.y);

	//通常状态
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	isClickReady = false;
	if (mousePosX >= posX && mousePosX<=posX + transform.size.x*currentScale
		&& mousePosY>=posY && mousePosY <= posY + transform.size.y*currentScale
		)
	{
		isSelect = true;//进入到被高亮状态
		
		if (!selectEventFlag) 
		{
			//高亮事件
			if (this->selectedAudio!=nullptr)
			{
				selectedAudio->Play(false);
			}
			this->selectedEvent.Invoke();
			selectEventFlag = true;
		}

		if (Input::GetMouseKeyHold(MouseButton::LEFT_BUTTON)) 
		{
			//进入点击状态
			isClickReady = true;
		}
		else if (Input::GetMouseKeyUp(MouseButton::LEFT_BUTTON))
		{
			if (clickAudio!=nullptr)
			{
				clickAudio->Play(false);
			}
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
			targetScale = heightLightScale;
		}
		else {
			currentColor = normalColor;
			currentTextColor = textNormalColor;
			targetScale = normalScale;
		}
	}
	else {
		currentColor = clickColor;
		currentTextColor = textClickColor;
		targetScale = clickScale;
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

	const float lerpSpeed = 2.0f;
	currentScale = noa::Math::LinearLerp(
		currentScale
		,targetScale
		, lerpSpeed
	);

	currentSize.x = transform.size.x * currentScale;
	currentSize.y = transform.size.y * currentScale;

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
		.SetClickAudio(button->clickAudio)
		.SetSelectedAudio(button->selectedAudio)
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

noa::Button& noa::Button::SetActive(bool value)
{
	UIComponent::SetActiveInContainer(value);
	return *this;
}

noa::Button& noa::Button::SetSize(int w,int h) 
{
	this->transform.size.x = w;
	this->transform.size.y = h;

	this->currentSize.x = static_cast<float>(transform.size.x);
	this->currentSize.y = static_cast<float>(transform.size.y);

	this->sprite.size.x = w;
	this->sprite.size.y = h;
	this->sprite.ResizeAndFull(w,h, noa::RGBA(255, 255, 255, 255));

	return *this;
}

noa::Button& noa::Button::SetRadius(int value)
{
	//四个角的半径

	this->sprite.Full(noa::RGBA(255, 255, 255, 255));
	if (value<=0) 
	{
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

	//左上角
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

	//左下角
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

	//右下角
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

	//右上角
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

noa::Button& noa::Button::SetSelectedAudio(
	std::shared_ptr<noa::AudioClip> audio) 
{
	this->selectedAudio = audio;
	return *this;
}

noa::Button& noa::Button::SetClickAudio(
	std::shared_ptr<noa::AudioClip> audio)
{
	this->clickAudio = audio;
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

noa::ProcessBar::ProcessBar(noa::UIContainer* container)
	:noa::UIComponent(container)
{
	background = noa::Image::Create(container);
	runtime = noa::Image::Create(container);

	SetSize(360, 20);

}

noa::ProcessBar::~ProcessBar() {

}

void noa::ProcessBar::UpdateRuntimeSprite() {
	//首先计算x进度
	const int targetX = static_cast<int>(amount * transform.size.x);
	runtimeSprite.Full(noa::RGBA(0, 0, 0, 0));
	for (int x = 0; x < targetX; x++)
	{
		for (int y = 0; y < runtimeSprite.h; y++)
		{
			runtimeSprite.SetPixelColor(
				x, y
				, noa::RGBA(255, 255, 255, 255));
		}
	}
}

void noa::ProcessBar::Start() {
	
}

void noa::ProcessBar::Update() 
{

	const int posX = static_cast<int>(transform.position.x + fatherTransform.position.x - anchor.x * transform.size.x);
	const int posY = static_cast<int>(transform.position.y + fatherTransform.position.y - anchor.y * transform.size.y);

	//通常状态
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	

	//交互
	//获取鼠标的位置和自己当前的global位置

	if (interactable
		&&Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON)) 
	{
		Vector<double> mousePos = Input::GetMousePosition();
		const int mousePosX = static_cast<int>(mousePos.x);
		const int mousePosY = static_cast<int>(mousePos.y);
		if (mousePosX >= posX-10 && mousePosX<=posX + transform.size.x+10
		&& mousePosY>=posY-5 && mousePosY <= posY + transform.size.y+5
		) 
		{
			//鼠标在范围内
			const float deltaX = static_cast<float>(mousePosX - posX);
			amount = deltaX / transform.size.x;
			if (amount<=0.0f)
			{
				amount = 0.0f;
			}
			else if(amount>=1.0f){
				amount = 1.0f;
			}
		}

	}

	if (amount >= 1.0f)
	{
		amount = 1.0f;
		if (!finished)
		{
			finishedEvent();
			finished = true;
		}
	}
	else if (amount < 1.0f)
	{
		finished = false;
	}

	//更新数据
	if (oldAmount == amount) 
	{
		return;
	}

	UpdateRuntimeSprite();
	SetRadius(radius);

	runtime->SetSprite(&runtimeSprite);
	oldAmount = amount;

}

void noa::ProcessBar::Render() {
	//绘制

	background->anchor = anchor;
	background->transform.position = transform.position;
	background->transform.size = transform.size;
	background->color = backgroundColor;

	runtime->anchor = anchor;
	runtime->transform.position = transform.position;
	runtime->transform.size = transform.size;
	runtime->color = fillColor;

}

noa::ProcessBar* noa::ProcessBar::Create(
	noa::UIContainer* container) 
{
	return new noa::ProcessBar(container);
}

noa::ProcessBar& noa::ProcessBar::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetPosition(int x,int y) 
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetAnchor(float x,float y)
{
	this->anchor.x = x;
	this->anchor.y = y;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetActive(bool value) 
{
	noa::UIComponent::SetActiveInContainer(value);
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetBackgroundColor(
	uint32_t color)
{
	this->backgroundColor = color;
	this->backgroundSprite.Full(color);
	background->SetSprite(&backgroundSprite);
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetFillColor(
	uint32_t color) {
	this->fillColor = color;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetFinishedCallback(
	std::function<void()> func)
{
	this->finishedEvent += func;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetSize(int x,int y) 
{
	transform.size.x = x;
	transform.size.y = y;

	backgroundSprite.w = x;
	backgroundSprite.h = y;
	backgroundSprite.ResizeAndFull(x,y,noa::RGBA(255,255,255,255));
	
	runtimeSprite.w = x;
	runtimeSprite.h = y;
	runtimeSprite.ResizeAndFull(x, y, noa::RGBA(0,0,0,0));

	background->SetSprite(&backgroundSprite);
	runtime->SetSprite(&runtimeSprite);

	return *this;

}

noa::ProcessBar& noa::ProcessBar::SetAmount(float amount) 
{
	this->amount = amount;
	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetRadius(int value)
{
	//四个角的半径

	this->backgroundSprite.Full(noa::RGBA(255,255,255,255));
	this->UpdateRuntimeSprite();
	if (value <= 0)
	{
		radius = 0;
		return *this;
	}

	const int maxValue = (backgroundSprite.w < backgroundSprite.h) ? (backgroundSprite.w / 2) : (backgroundSprite.h / 2);
	radius = value;
	if (radius > maxValue)
	{
		radius = maxValue;
	}

	const int x1 = radius;
	const int x2 = backgroundSprite.w - radius;
	const int y1 = radius;
	const int y2 = backgroundSprite.h - radius;

	const int sqrRadius = (radius) * (radius);

	//左上角
	for (int x = 0; x < radius; x++)
	{
		for (int y = 0; y < radius; y++)
		{
			const int deltaX = x - x1;
			const int deltaY = y - y1;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) {
				backgroundSprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
				runtimeSprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	//左下角
	for (int x = 0; x < radius; x++)
	{
		for (int y = y2; y < backgroundSprite.h; y++)
		{
			const int deltaX = x - x1;
			const int deltaY = y - y2;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) {
				backgroundSprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
				runtimeSprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	//右下角
	for (int x = x2; x < backgroundSprite.w; x++)
	{
		for (int y = y2; y < backgroundSprite.h; y++)
		{
			const int deltaX = x - x2;
			const int deltaY = y - y2;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius) {
				backgroundSprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
				runtimeSprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));

			}
		}
	}

	//右上角
	for (int x = x2; x < backgroundSprite.w; x++)
	{
		for (int y = 0; y < radius; y++)
		{
			const int deltaX = x - x2;
			const int deltaY = y - y1;

			if (deltaX * deltaX + deltaY * deltaY > sqrRadius)
			{

				backgroundSprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
				runtimeSprite.SetPixelColor(x, y, RGBA(0, 0, 0, 0));
			}
		}
	}

	background->SetSprite(&backgroundSprite);
	runtime->SetSprite(&runtimeSprite);

	return *this;
}

noa::ProcessBar& noa::ProcessBar::SetInteractable(bool interactable)
{
	this->interactable = interactable;
	return *this;
}

noa::ProcessBar* noa::ProcessBar::Apply() {
	return this;
}

float noa::ProcessBar::GetValue() {
	return this->amount;
}


