function NoaEngineWeb()
{
    this.width = 800;
    this.height = 600;
    this.run = (nowFrame)=>
    {
        //获取到画布
        this.canvas = document.getElementById('canvas');
        this.ctx = this.canvas.getContext('2d');
        //给画布上
        this.ctx.fillStyle = 'green';
        this.ctx.fillRect(0,0,this.width,this.height);

        this.lastTick = 0;
        this.deltaTime = 0;
        this.start();
        this.engineLoop(nowFrame);
        this.onDisable();
    }

    this.start = ()=>{

    }

    this.update = ()=>{
        
    }

    this.onDisable = ()=>{

    }

    this.engineLoop = (nowFrame)=>{

        this.deltaTime = (nowFrame - this.lastTick)*0.001;

        //执行所有Actor的update
        this.update();
        requestAnimationFrame(this.engineLoop);
        //renderer
        this.lastTick = nowFrame;
    }


}

function main(){
    let game = new NoaEngineWeb();
    game.run(performance.now());
}

main();