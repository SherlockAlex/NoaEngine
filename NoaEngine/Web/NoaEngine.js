import { WebRenderer } from './WebRenderer';

class NoaEngine{

    //这样定义的类中，方法中的this指的是NoaEngine
    constructor(width,height)
    {
        //类的构造函数
        this.width = width;
        this.height = height;
        this.renderer = new WebRenderer();
    }
    
    Run(){
        console.log("Hello Engine",this);
        this.Start();
    }

    Start(){
        this.renderer.DrawRect(10,10,100,100,"green");
    }

};

let engine = new NoaEngine(1920,1080);
engine.Run();