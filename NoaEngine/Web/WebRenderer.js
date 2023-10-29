export class WebRenderer{
    constructor(){
        this.canvas = document.getElementById("canvas");
        this.context = this.canvas.getContext("2d");
    }

    DrawRect(x,y,width,height,color)
    {
        this.context.fillStyle = color;
        this.context.fillRect(x,y,width,height);
    }

}