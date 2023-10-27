function NoaEngineWeb()
{
    this.run = function()
    {
        this.start();
        this.engineLoop();
        this.onDisable();
    }

    this.start = function(){

    }

    this.update = function(){
        
    }

    this.onDisable = function(){

    }

    this.engineLoop = function(){
        this.update();
        requestAnimationFrame(this.engineLoop());
    }


}

function main(){
    let game = new NoaEngineWeb();
    game.run();
}

main();