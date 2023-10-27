function createEngineInstance()
{
    let engineInstance = new Object();
    
    engineInstance.run = function(){
        console.log("hello engine");
    };

    return engineInstance;
};

var engine = createEngineInstance();
engine.run();