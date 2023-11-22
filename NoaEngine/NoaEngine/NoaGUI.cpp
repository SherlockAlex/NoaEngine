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

	//�����߼�ֻ����ջ��
	if (!containerStack.empty()) 
	{
		containerStack.top()->Update();
	}
	
}

void noa::UIDocument::UIDocumentRender() {
	for (auto& container : containerList)
	{
		//Խ��ǰ��Խ�Ȼ���
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

	//ɾ���ؼ�
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
	//��ʾ����
	
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

	//��ʾ����
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
		//��component��������
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
	//����ֵ��Ȼ�����position
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
	//��ʾ����

	

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
	//�������spriteGPU
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

	//button��״̬ת������

	if (!active)
	{
		return;
	}

	const Vector<double>& mousePos = Input::GetMousePosition();

	const float mousePosX = (static_cast<float>(mousePos.x));
	const float mousePosY = (static_cast<float>(mousePos.y));

	const int posX = static_cast<int>(transform.position.x + fatherTransform.position.x - anchor.x * transform.size.x);
	const int posY = static_cast<int>(transform.position.y + fatherTransform.position.y - anchor.y * transform.size.y);

	//ͨ��״̬
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	isClickReady = false;
	if (mousePosX >= posX && mousePosX<=posX + transform.size.x*currentScale
		&& mousePosY>=posY && mousePosY <= posY + transform.size.y*currentScale
		)
	{
		isSelect = true;//���뵽������״̬
		
		if (!selectEventFlag) 
		{
			//�����¼�
			if (this->selectedAudio!=nullptr)
			{
				selectedAudio->Play(false);
			}
			this->selectedEvent.Invoke();
			selectEventFlag = true;
		}

		if (Input::GetMouseKeyHold(MouseButton::LEFT_BUTTON)) 
		{
			//������״̬
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
	//�ĸ��ǵİ뾶

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

	//���Ͻ�
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

	//���½�
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

	//���½�
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

	//���Ͻ�
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
	//���ȼ���x����
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

	//ͨ��״̬
	globalTransform.position.x = posX;
	globalTransform.position.y = posY;

	

	//����
	//��ȡ����λ�ú��Լ���ǰ��globalλ��

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
			//����ڷ�Χ��
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

	//��������
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
	//����

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
	//�ĸ��ǵİ뾶

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

	//���Ͻ�
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

	//���½�
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

	//���½�
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

	//���Ͻ�
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


