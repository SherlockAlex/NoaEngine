# NoaEngine
## 简介
目前这个游戏引擎正在开发中，目前项目中使用这个游戏引擎开发出老牌的《德军总部3D》风格的第一人称射击游戏。
如果你想使用这个游戏引擎开发出你自己的游戏，首先你得会使用C++及其面向对象的相关语法。
在NoaEngine.h中有我们的一个简单的项目模板，你可以仿照它，快速的创建一个游戏项目示例。
你需要注意的是，这个游戏引擎使用SDL2图形库作为底层的图形接口，所以你需要在你的电脑上先安装SDL2相关的开发库。
我们推荐Windows平台的用户使用Visual Studio 2022作为开发平台，导入游戏引擎相关的文件到头文件中便可。
而Linux平台下的用户，你可以详细阅读并修改Makefile中的内容，使用其中我们已经编写好的make命令来配置开发环境和编译你的游戏。
游戏引擎目前只在Windows平台测试过，对于跨平台特性，由于本人技术有限，并未完成。

## 引擎内容
* Platform:抽象类，一个平台一个类。

* NoaEngine:结合Platform，实现跨平台，此为抽象类，你必须创建属于你自己的游戏主类，同时实现抽象方法:virtual void Start()、virtual void Update()。

* Scene:游戏场景，此为非必须类，但是如果你想使用游戏引擎提供的Animation，Actor，GameObject等游戏组件，你必须编写你自己的场景子类，并编写相应场景的活动规则，只要实例化场景对象，便会自动在sceneManager中的场景列表自动注册场景，同时游戏引擎会自动执行sceneManager.activeScene的内容。

* Actor:游戏活动物体，只能使用Create()进行实例化对象，并分配到堆空间上，其中，Actor必须先持有一个Scene * activeScene才能执行，实例化后会自动在activeScene的actors列表中自动登记，并每一帧去执行Actor的内容。

* SpriteRenderer:精灵绘制组件，持有一个Sprite，如果场景拥有一个相机，相机会自动渲染Sprite到屏幕的特定上，通常用于渲染2D游戏角色。

* Animation:动画组件，是Actor的子类，初始化后必须执行SetActiveScene(Scene * activeScene)才能够运行动画，注意Animation实例化后，必须先持有一个AnimationFrame对象，AnimationFrame对象可以读取并加载本地动画文件(*.amt)。

* Camera:相机，不同种类的相机可以让你快速渲染游戏画面，详细请见void Camera::Render(/*args*/)方法说明。

* Sprite:精灵，使用CPU渲染图片到屏幕上。

* Renderer:渲染器抽象类，一个图形API对应一个抽象子类。

* GLTexture:OpenGL纹理，创建OpenGL纹理使用GPU加速渲染图片。

* GLRenderer:基于OpenGL的渲染器。

* SpriteGL:使用GPU加速Sprite的绘制。

* Rigidbody:刚体组件，用于实现物理模拟。

* InputSystem:输入系统，实现游戏的输入控制。
