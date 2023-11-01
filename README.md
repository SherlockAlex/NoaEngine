# NoaEngine
## 简介
目前这个游戏引擎正在开发中，目前项目中使用这个游戏引擎开发出老牌的《德军总部3D》风格的第一人称射击游戏。
如果你想使用这个游戏引擎开发出你自己的游戏，首先你得会使用C++及其面向对象的相关语法。
在NoaEngine.h中有我们的一个简单的项目模板，你可以仿照它，快速的创建一个游戏项目示例。
你需要注意的是，游戏引擎支持使用OpenGL和SDL2图形库作为底层的图形接口，跨平台方面底层采用SDL_Window作为图形窗口开发，所以你需要在你的电脑上先安装SDL2相关的开发库。
我们推荐Windows平台的用户使用Visual Studio 2022作为开发平台，导入游戏引擎相关的文件到头文件中便可。
而Linux平台下的用户，你可以详细阅读并修改Makefile中的内容，使用其中我们已经编写好的make命令来配置开发环境和编译你的游戏。
游戏引擎目前只在Windows平台测试过，对于跨平台特性，由于本人技术有限，并未完成。

## 环境依赖
* windows平台建议使用VS2022
* SDL2
* glfw (NupenGL)
* freetype2

## 重大BUG
* 使用OpenGL渲染时候，无法正确渲染出图片

## 引擎内容
* Platform:抽象类，一个平台对应一个Platform派生类，比如引擎已经实现的Platform_Windows类，利用SDL_Window创建图形化窗口。

* NoaEngine:结合Platform，实现跨平台，此为抽象类，你必须创建属于你自己的游戏主类，同时实现抽象方法:virtual void Start()、virtual void Update()。

* Scene:游戏场景，此为非必须类，但是如果你想使用游戏引擎提供的ActorComponent，Actor等游戏组件，你必须编写你自己的场景子类，并编写相应场景的活动规则，只要实例化场景对象，便会自动在sceneManager中的场景列表自动注册场景，同时游戏引擎会自动执行sceneManager.activeScene的内容，Scene管理着你游戏里的所有Actor，没有它，就没有办法让Actor们执行相应的功能。

* Actor:游戏活动物体，只能使用Create()或者NObject::Create()进行实例化对象，并分配到堆空间上，其中，Actor必须先持有一个Scene * activeScene才能执行，实例化后会自动在activeScene的actors列表中自动登记，并每一帧去执行Actor的内容，只有Actor才能在Scene中活动，同时Actor还负责管理自己身上的ActorComponent的运行，创建和销毁。

* ActorComponent:Actor组件，是非常重要的工具，用户可以自定义自己的组件，然后挂在到Actor的身上，让Actor获得某项重要的能力。

* SpriteRenderer:精灵绘制组件，是ActorComponent的子类，持有一个Sprite，如果场景拥有一个相机，相机会自动渲染Sprite到屏幕的特定上，通常用于渲染2D游戏角色。

* AnimationFrame:本地动画文件*.amt的对应动画帧结构体，保存本地动画文件的所有数据，可以使用resource.LoadAnimationFile("amt file path")读取动画数据，获得结构体AnimationFile，然后利用AnimationFile构建AnimationFrame数据，将AnimationFrame地址传给动画组件便可。

* Animation:动画组件，是Actor的子类，初始化后必须执行SetActiveScene(Scene * activeScene)才能够运行动画，注意Animation实例化后，必须先持有一个AnimationFrame对象，AnimationFrame对象可以读取并加载本地动画文件(*.amt)。

* Camera:相机，不同种类的相机可以让你快速渲染游戏画面，详细请见void Camera::Render(/*args*/)方法说明。

* SpriteFile:本地精灵文件*.spr对应的结构体，可以通过resource.LoadSprFile("spr file path")加载本地精灵文件并返回其数据。

* Sprite:精灵，保存图片的信息到内存中，其成员方法支持以CPU的形式渲染图片到屏幕上，可以通过加载本地的SpriteFile进行初始化创建精灵。

* Renderer:渲染器抽象类，一个图形API对应一个其派生类，比如GLRenderer是OpenGL的Renderer具体实现，可以调用OpenGL进行图像绘制。

* GLTexture:OpenGL纹理，创建OpenGL纹理使用GPU加速渲染图片，是抽象类Texture的具体实现，可以创建OpenGL纹理对象进行绘制图像。

* GLRenderer:基于OpenGL的渲染器，是Renderer抽象类的OpenGL具体实现。

* SpriteGL:使用GPU加速Sprite的绘制。

* Rigidbody:刚体组件，用于实现物理模拟。

* PhysicsSystem:物理系统，负责构建物理世界，并处理各种物理行为。

* SceneManager:场景管理器，负责场景的切换，卸载，运行和销毁等功能

* InputSystem:输入系统，实现游戏的输入控制。

* ObjectPool:对象池模板类，可以通过继承的方式，可以快速构建一个对象池，注意在创建对象池之前，你必须创建对应的IPoolObjectFactory，游戏引擎提供了ActorPool的实现，通过继承ActorPool，可以更加有效的复用游戏资源，注意你必须在使用Pool之前，要给Pool绑定一个Factory，Pool才能正常工作。

* IPoolObjectFatory:对象池物体创建工厂，主要负责游戏物品的对象的生成，你必须实现无参数抽象方法 T* Create()。

* NOAObject:游戏引擎最最基本的类，所有Actor和ActorComponent的基类，主要用于标记哪些对象可以被游戏引擎进行管理，以及便于API的统一调度。

* NObject:游戏引擎的工具类，主要用于快速创建Actor和ActorCopoment用户自定义的游戏对象，因为两者对象要求只能分配到堆上。用户只要在自己编写的Actor和Component中使用宏NOBJECT(类名)，便可以使用NObject快速创建对象。
