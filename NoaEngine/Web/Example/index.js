class Vector2{
    constructor(x,y)
    {
        this.x = x;
        this.y = y;
    }

}

class Transform{
    constructor(){
        this.position = new Vector2(0,0);
        this.scale = new Vector2(1,1);
        this.eulerAngle = 0.0;
    }
}

class Renderer{
    static init = ()=>{
        Renderer.canvas = document.getElementById("canvas");
        if(Renderer.canvas == null)
        {
            console.log("当前浏览器不支持canvas");
        }

        Renderer.context = Renderer.canvas.getContext("2d");
    }

    static clear = ()=>{
        Renderer.context.clearRect(
            0
            ,0
            ,Renderer.canvas.width
            ,Renderer.canvas.height
        );
    }

    static drawRect=(position,scale,color)=>
    {
        Renderer.context.fillStyle = 'green';
        Renderer.context.fillRect(position.x,position.y,scale.x,scale.y);
        console.log(position,scale);
    }

}

class Time{
    static deltaTime = 0.0;
    static lastTick = 0.0;
}

class InputSystem{
    static on = (eventType,callback)=>
    {
        document.addEventListener(eventType,callback);
    }
}

class ActorComponent{
    constructor(actor){
        if(actor == null)
        {
            console.error("挂载组件失败");
        }
        actor.addComponent(this);
        this.actor = actor;
    }

    start = ()=>{

    }

    update = ()=>{
        //每个Update中执行一次
    }

}

class Actor{
    constructor(){
        this.transform = new Transform;
        this.components = [];
    }

    addComponent =(component)=>
    {
        this.components.push(component);
    }

    componentStart=()=>{
        for(component in this.components)
        {
            if(component == null)
            {
                continue;
            }
            component.start();
        }
    }

    componentUpdate=()=>{
        for(component in this.components)
        {
            if(component == null)
            {
                continue;
            }
            component.update();
        }
    }

    

}

//游戏主类
class NoaEngine{
    constructor(){
        Renderer.init();
    }

    onLoad = ()=>{

    }

    update=()=>{

    }

    render=()=>{
        Renderer.clear()
        // 渲染所有的renderable
        //Renderer.drawRect(this.position,this.scale,'green');
    }

    run =()=>{
        this.onLoad();
        this.engineThread(performance.now());
        
    }

    engineThread = (tFrame)=>
    {
        Time.deltaTime = (tFrame - Time.lastTick)*0.001;
        this.update();
        this.render();
        Time.lastTick = tFrame;
        requestAnimationFrame(this.engineThread);
    }

}

class Game extends NoaEngine
{
    constructor(){
        super();
    }

    onLoad = ()=>{

    }

    update=()=>{
        
    }

}

let game = new Game();
game.run();