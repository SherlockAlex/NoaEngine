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
	if (groups.top() == nullptr) 
	{
		groups.pop();
	}
	else {
		groups.top()->Update();
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
	//显示背景
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

noa::Label& noa::Label::SetColor(uint32_t color) 
{
	this->color = color;
	return *this;
}

noa::Label& noa::Label::SetFontSize(uint32_t size)
{
	this->size = size;
	this->transform.scale = renderer->GetLabelScale(this->text,this->size);
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
	this->transform.scale = renderer->GetLabelScale(this->text, this->size);
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
	//显示文字
	const int posX = fatherTransform.position.x + transform.position.x;
	const int posY = fatherTransform.position.y + transform.position.y;
	renderer->DrawString(
		text
		, posX
		, posY
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

noa::Image& noa::Image::SetScale(int x, int y) 
{
	this->transform.scale.x = x;
	this->transform.scale.y = y;
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
		//如果有了spriteGPU
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

	if (sprite == nullptr)
	{
		return;
	}
	
	const float posX = static_cast<float>(fatherTransform.position.x + transform.position.x);
	const float posY = static_cast<float>(fatherTransform.position.y + transform.position.y);

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
			posX
			, posY
			, static_cast<float>(transform.scale.x)
			, static_cast<float>(transform.scale.y)
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
	
	this->SetSize(80, 40);
	/*this->transform.scale = { 80,40 };*/
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

	//下面函数在全屏时会出现bug，需要解决

	if (!active)
	{
		return;
	}

	const Vector<double>& mousePos = Input::GetMousePosition();

	const float mousePosX = (static_cast<float>(mousePos.x));
	const float mousePosY = (static_cast<float>(mousePos.y));

	const int posX = transform.position.x + fatherTransform.position.x;
	const int posY = transform.position.y + fatherTransform.position.y;

	isClickReady = false;
	if (mousePosX > posX && mousePosX<posX + transform.scale.x
		&& mousePosY>posY && mousePosY < posY + transform.scale.y
		)
	{
		isSelect = true;
		
		if (Input::GetMouseKeyHold(MouseButton::LEFT_BUTTON)) 
		{
			isClickReady = true;
		}
		else if (Input::GetMouseKeyUp(MouseButton::LEFT_BUTTON))
		{
			this->clickEvent.Invoke();
		}
		

	}
	else {
		isSelect = false;
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

	currentSize.x = static_cast<int>(transform.scale.x * currentScale);
	currentSize.y = static_cast<int>(transform.scale.y * currentScale);

	this->label->SetFontSize(static_cast<uint32_t>(fontSize * currentScale));

	image->transform.position = transform.position;
	image->transform.scale = currentSize;
	image->color = currentColor;

	label->color = currentTextColor;
	label->transform.position.x =
		static_cast<int>(
			transform.position.x
			+ 0.5f * currentSize.x
			- 0.5f * label->transform.scale.x);
	label->transform.position.y =
		static_cast<int>(
			transform.position.y
			+ 0.5f * currentSize.y
			- 0.5f * label->transform.scale.y);
}

noa::Button& noa::Button::Clone(Button* button) {
	return this->SetNormalColor	(button->normalColor)
		.SetHeightLightColor		(button->heightLightColor)
		.SetClickColor			(button->clickColor)
		.SetSize					(button->transform.scale.x, 
									button->transform.scale.y)
		.SetRadius				(button->radius)
		.SetTextNormalColor		(button->textNormalColor)
		.SetTextHeightLightColor	(button->textHeightLightColor)
		.SetTextClickColor		(button->textClickColor)
		.SetFontSize				(button->fontSize)
		.SetNormalScale			(button->normalScale)
		.SetHeightLightScale		(button->heightLightScale)
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

noa::Button& noa::Button::SetSize(int w,int h) 
{
	this->transform.scale.x = w;
	this->transform.scale.y = h;

	this->currentSize = transform.scale;

	SpriteFile spriteFile;
	spriteFile.width = w;
	spriteFile.height = h;
	spriteFile.images.resize(spriteFile.width * spriteFile.height, noa::RGBA(255, 255, 255, 255));
	
	this->sprite.scale.x = w;
	this->sprite.scale.y = h;
	this->sprite.UpdateImage(spriteFile);

	SetRadius(this->radius);

	return *this;
}

noa::Button& noa::Button::SetRadius(int value)
{
	//四个角的半径
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

noa::Button& noa::Button::AddClickCallback(std::function<void()> func)
{
	this->clickEvent += func;
	return *this;
}

noa::Button* noa::Button::Apply() {
	return this;
}


