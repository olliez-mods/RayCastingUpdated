#include "SFAS/Widgets.hpp"
#include "SFAS/Conversion.hpp"
#include "SFAS/dt.hpp"
#include "SFAS/RpFinder.hpp"
#include "SFAS/Addons/Scroller/Scroller.hpp"

#include "sockets.hpp"

#include <random>
#include<ctime>
#include <iostream>
#include <filesystem>

#define PI 3.14159265358979323846264338327950288419716939937510
#define ResX 600
#define ResY 600

#define WinX 600
#define WinY 600

// in degrees
#define FOV 90.0

#define MiniMapX 150
#define MiniMapY 150

#define camHeight 20
#define wallHeight 80


using sfas::c;
using sfas::rp;

namespace fs = std::filesystem;

struct Material{
    sf::Image image;
    std::string name;
    bool transparent;
    bool passable;
    Material(std::string name, sf::Image image, bool transparent, bool passable){
        this->name = name;
        this->image = image;
        this->transparent = transparent;
        this->passable = passable;
    }
    
    Material(){
        name = "blank";
        image = sf::Image();
        transparent = false;
        passable = false;
    }
};

std::string getRandomTag(){
    srand(time(0));
    return std::to_string((1000 + (rand() % (9999-1000))));
}


struct Wall{
    sf::Vector2f pos1 = sf::Vector2f(-1, -1);
    sf::Vector2f pos2 = sf::Vector2f(-1, -1);
    
    std::string tag = "0000";
    
    bool transparent = false;
    bool passThrough = false;
    
    Material material = Material();
    
    //std::string texture;
    
    Wall(){}
    Wall(std::string tag, sf::Vector2f pos1, sf::Vector2f pos2, Material material){
        this->pos1 = pos1;
        this->pos2 = pos2;
        this->material = material;
        transparent = material.transparent;
        passThrough = material.passable;
        
        this->tag = tag;
        if(tag == "-1"){
            this->tag = getRandomTag();
        }
    }
};

struct Hit{
    bool hit = false;
    float dist = -1;
    float trueDist = -1;
    sf::Vector2f posF = sf::Vector2f(-1, -1);
    sf::Vector2i posI = sf::Vector2i(-1, -1);
    Wall *wall;
    float angle = -1;
};

struct Result{
    bool hit = false;
    std::vector<Hit> hits;
    float angle = -1;
    
};
Result cast(sf::Vector2f pos, float playerAngle, float angle, std::vector<Wall>& walls)
{
    Result res;
    res.angle = angle;
    
    // Loop throgh all the walls and save EVERY hit into result
    for(int i = 0; i < walls.size(); i++){
        
        int x1 = walls[i].pos1.x;
        int y1 = walls[i].pos1.y;
        int x2 = walls[i].pos2.x;
        int y2 = walls[i].pos2.y;
        
        int x3 = pos.x;
        int y3 = pos.y;
        
        float x4 = pos.x + cos(playerAngle + angle);
        float y4 = pos.y + sin(playerAngle + angle);
        
        // I changed den to a float (which seemed to make things better) if you hav einacuracy errors maybe change it back to an int
        float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        if(den == 0){
            //No hit
            continue;
        }
        
        float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / (float) den;
        float u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / (float) den;
        
        if(t >= 0 && t <= 1 && u > 0)
        {
            res.hit = true;
            
            sf::Vector2f hitPos = sf::Vector2f(x1 + t*(x2 - x1), y1 + t*(y2 - y1));
            float dist = std::sqrt(std::pow(hitPos.x-pos.x, 2) + std::pow(hitPos.y-pos.y, 2));
            
            res.hits.push_back(Hit());
            res.hits[res.hits.size()-1].hit = true;
            res.hits[res.hits.size()-1].posF = hitPos;
            res.hits[res.hits.size()-1].posI = sf::Vector2i(hitPos);
            res.hits[res.hits.size()-1].dist = dist;
            res.hits[res.hits.size()-1].angle = angle;
            res.hits[res.hits.size()-1].wall = &walls[i];
            res.hits[res.hits.size()-1].trueDist = dist*cos(angle);
            
        }
    }
    
    
    // Sort through the rays and discard any ray that's behind a solid (non transparent wall)
    
    
    // We want to get at most the closest 4 walls
    if(res.hit){
        for(int r = 0; r < 4 && r < res.hits.size(); r++){
            
            //Get the closest ray Note, we start at r so that we dont sort through rays that are already sorted
            
            float shortestDist = INFINITY;
            int indexOfClosestHit = -1;
            for(int i = r; i < res.hits.size(); i++){
                if(res.hits[i].dist < shortestDist){
                    shortestDist = res.hits[i].dist;
                    indexOfClosestHit = i;
                }
            }
            Hit temp = res.hits[r];
            res.hits[r] = res.hits[indexOfClosestHit];
            res.hits[indexOfClosestHit] = temp;
            
            // if current wall is solid then ignore anything else
            // this means walls will get rendered even behind other solid walls
            /*
             if(!res.hits[r].wall->transparent && r != res.hits.size()-1){
             if(r == 0)
             res.hits = std::vector<Hit>(res.hits.begin(), res.hits.begin()+1);
             else
             res.hits = std::vector<Hit>(res.hits.begin(), res.hits.begin()+r+1);
             break;
             }
             //*/
        }
        
    }
    
    
    
    return res;
}













Material& getMaterialByName(std::vector<Material> &materials, std::string name){
    for(int i = 0; i < materials.size(); i++){
        if(materials[i].name == name)
            return materials[i];
    }
    // the texture wasnt found so now get missing texture
    if(name != "missing"){
        return getMaterialByName(materials, "missing");
    }
    // we were tasked to get the missing texture but couldnt find it, return default (empty, probably will make errors ;-;)
    std::cout << "Couldn't find missing texture, please add \"00missing.png\" file to the textures folder\n";
    // add a blank material to the vector if its empty, then return the first element of the vector
    if(materials.size() == 0){
        materials.push_back(Material());
        materials[materials.size()-1].name = "test";
    }
    
    return materials[0];
}

int getWallIndexByTag(std::vector<Wall>& walls, std::string tag){
    for(int i = 0; i < walls.size(); i++){
        if(walls[i].tag == tag)
            return i;
    }
    return -1;
}

bool removeWallByTag(std::vector<Wall>& walls, std::string tag){
    for(int i = 0; i < walls.size(); i++){
        if(walls[i].tag == tag){
            walls.erase(walls.begin()+i);
            return true;
        }
    }
    return false;
}
















int main(){
    //sfas::setResPath("content/Resources/");
    //*
    // setting up the windows
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WinX, WinY+150)), "Rooms");
    sf::RenderWindow materialWindow(sf::VideoMode(sf::Vector2u(400, 750)), "Materials");
    materialWindow.setPosition(sf::Vector2i(window.getPosition().x-materialWindow.getSize().x, window.getPosition().y));
    
    
    sfas::WidgetManager widgetMan;
    sfas::Widget::setFont(rp() + "sansation.ttf");
    
    sf::RectangleShape background(c(WinX, WinY+150));
    background.setPosition(c(0, 0));
    background.setFillColor(sf::Color(50, 50, 50));
    
    sfas::Canvas veiwCanvas(c(0, 0), c(WinX, WinY));
    veiwCanvas.create(c(ResX, ResY));
    widgetMan.registerWidget(&veiwCanvas);
    
    sfas::Canvas mapCanv(c(WinX-150, WinY), c(150, 150));
    mapCanv.create(c(MiniMapX, MiniMapY));
    widgetMan.registerWidget(&mapCanv);
    
    
    sfas::Slider slider(c(10, WinY+10), c(100, 25), 0, 200);
    slider.showValueAsInt = true;
    widgetMan.registerWidget(&slider);
    
    sfas::Input ipInput(c(WinX-280, WinY+10), c(120, 30));
    ipInput.setTextSize(15);
    widgetMan.registerWidget(&ipInput);
    
    sfas::Button buttin(c(WinX-280, WinY+50), c(120, 30));
    buttin.setColor(sf::Color::Red);
    buttin.setText("Connect", sf::Color::Black, 20);
    widgetMan.registerWidget(&buttin);
    
    //Widget manager for the material window
    sfas::WidgetManager materialWidgetMan;
    
    std::vector<sfas::Button> materialButtons;
    
    std::vector<Material> materials;
    
    //std::vector<sf::Image> materials;
    //std::vector<std::string> materialNames;
    //std::unordered_map<std::string, int> materialIds;
    //std::vector<std::string> materialAttribs;
    
    
    //x: (round((size/2.0)-(int)(size/2)))*height- + 25, y: ((int)size/2)*height + 25
    
    
    //Read the files in the textures folder and assigne each one with a button and eahc button with the texture
    std::string path = rp() + "textures";
    try {
        const int COLLUMNS = 4;
        const int BUFFER = 10;
        const int WIDTH = materialWindow.getSize().x;
        
        int collumn = 0;
        int row = 0;
        
        
        
        
        for (const auto & entry : fs::directory_iterator(path)){
            
            if(entry.path().filename().extension().string() == ".png"){
                std::cout << "Loading: " << entry.path() << std::endl;
                
                
                if(collumn >= COLLUMNS){
                    collumn = 0;
                    row += 1;
                }
                
                
                
                sf::Image img;
                img.loadFromFile(entry.path());
                
                sf::Texture texture;
                texture.loadFromImage(img);
                
                sfas::Button button(c(BUFFER + collumn*(WIDTH/COLLUMNS), BUFFER + row*((WIDTH/COLLUMNS-BUFFER)/3*2 + BUFFER)), c(WIDTH/COLLUMNS-BUFFER, (WIDTH/COLLUMNS-BUFFER)/3*2));
                button.setTexture(texture);
                collumn += 1;
                
                std::string name = entry.path().stem().string().substr(2, entry.path().stem().string().size()-1);
                std::string attribs = entry.path().stem().string().substr(0, 2);
                
                materialButtons.push_back(button);
                materials.push_back(Material(name, img, attribs[0] == '1', attribs[1] == '1'));
                /*
                 materials.push_back(img);
                 materialIds[name] = (int)materialButtons.size()-1;
                 materialNames.push_back(name);
                 materialAttribs.push_back(attribs);
                 */
            }
        }
        
    } catch (...) {
        std::cout << "error reading textures from \"" + rp()+"textures\"\n";
    }
    
    for(int i = 0; i < materialButtons.size(); i++){
        materialWidgetMan.registerWidget(&materialButtons[i]);
    }
    std::cout << "Loaded " << materialButtons.size() << " textues\n";
    
    
    
    
    
    
    
    sf::Vector2f playerPos(50, 50);
    sf::Vector2f playerLoadedStep(0, 0);
    
    std::vector<Wall> walls;
    //std::unordered_map<std::string, int> wallTags;
    
    /*
     walls.push_back(Wall(c(0, 0), c(100, 0), "wall", false));
     walls.push_back(Wall(c(0, 0), c(0, 100), "wall", false));
     walls.push_back(Wall(c(0, 0), c(0, 100), "wall", false));
     walls.push_back(Wall(c(100, 0), c(100, 50), "wall", false));
     walls.push_back(Wall(c(100, 50), c(100, 100), "window", true));
     walls.push_back(Wall(c(0, 100), c(52, 100), "window", true));
     walls.push_back(Wall(c(48, 100), c(77, 100), "wall", false));
     walls.push_back(Wall(c(73, 100), c(100, 100), "door", true, true));
     walls.push_back(Wall(c(25, 10), c(75, 10), "painting", true));
     */
    
    // In radians
    float playerAngle = 0;
    
    std::vector<Result> results;
    Result backRay;
    
    bool drawingWall = false;
    sf::Vector2f startPos;
    //std::string wallTag;
    
    bool canDelWall = true;
    
    int selectedMaterialIndex = 0;
    
    int msTmout = 100;
    float total = 0;
    
    
    
    
    
    
    
    
    
    //materialWindow.close();
    
    while(window.isOpen())
    {
        sf::Event event;
        
        // main window events
        while(window.pollEvent(event))
        {
            widgetMan.checkEventForStack(event);
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                window.close();
                materialWindow.close();
            }
        }
        
        // material window events
        while(materialWindow.pollEvent(event))
        {
            materialWidgetMan.checkEventForStack(event);
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                window.close();
                materialWindow.close();
            }
        }
        
        
        widgetMan.tickStack(window);
        materialWidgetMan.tickStack(materialWindow);
        double dt = resetDT();
        
        
        
        
        
        //==============
        // TICK THE SERVER... and other server stuff
        //==============
        tickServer();
        
        
        if(buttin.clickedOnTick){
            if(getConnected()){
                
                disconnect();
                buttin.setColor(sf::Color::Red);
                buttin.setText("Connect", sf::Color::Black, 20);
                
            }else{
                
                if(ipInput.getText() == ""){
                    ipInput.setText("localhost");
                }
                
                if(connect(ipInput.getText())){
                    walls.clear();
                    sf::Packet p;
                    p << "server" << "getAllWalls";
                    send(p);
                    
                    buttin.setColor(sf::Color::Green);
                    buttin.setText("Disconnect", sf::Color::Black, 20);
                }
            }
        }
        
        if(checkNewData()){
            sf::Packet data = getData();
            
            std::string command;
            
            data >> command;
            if(command == "message"){
                std::string msg;
                data >> msg;
                
                std::cout << "Received message: " << msg << "\n";
            }
            if(command == "newWall"){
                std::string tag;
                sf::Vector2f pos1;
                sf::Vector2f pos2;
                std::string materialName;
                data >> tag >> pos1.x >> pos1.y >> pos2.x >> pos2.y >> materialName;
                std::cout << "new wall with tag: " << tag << ",   and texture name: " << materialName << "\n";
                walls.push_back(Wall(tag, pos1, pos2, getMaterialByName(materials, materialName)));
            }
            if(command == "removeWall"){
                std::string tag;
                data >> tag;
                removeWallByTag(walls, tag);
            }
            if(command == "sendWalls"){
                sf::Packet p;
                setBlocking(true);
                for(int i = 0; i < walls.size(); i++){
                    p.clear();
                    p << "server" << "groupWalls" << walls[i].tag << walls[i].pos1.x << walls[i].pos1.y << walls[i].pos2.x << walls[i].pos2.y << walls[i].material.name;
                    send(p);
                }
                setBlocking(false);
            }
            if(command == "moveWall"){
                std::string tag;
                sf::Vector2f pos1;
                sf::Vector2f pos2;
                data >> tag >> pos1.x >> pos1.y >> pos2.x >> pos2.y;
                
                int ind = getWallIndexByTag(walls, tag);
                
                if(ind != -1){
                    walls[ind].pos1 = pos1;
                    walls[ind].pos2 = pos2;
                }
            }
        }
        
        
        
        
        
        //===============
        // CREATING WALLS
        //===============
        {
#define acc 10
            
            // this finds the closest wall edge to the given position, if its below acc threshold then snap the returned position to it
            auto getClosestPoint = [walls](sf::Vector2f point, sf::Vector2f exclude){
                sf::Vector2f closest = c(INFINITY, INFINITY);
                float closestDist = INFINITY;
                
                for(int i = 0; i < walls.size(); i++)
                {
                    
                    float dist1 = abs(std::sqrt(std::pow(point.x-walls[i].pos1.x, 2) + std::pow(point.y-walls[i].pos1.y, 2)));
                    float dist2 = abs(std::sqrt(std::pow(point.x-walls[i].pos2.x, 2) + std::pow(point.y-walls[i].pos2.y, 2)));
                    
                    if(dist1 < closestDist){
                        closestDist = dist1;
                        closest = walls[i].pos1;
                    }
                    if(dist2 < closestDist){
                        closestDist = dist2;
                        closest = walls[i].pos2;
                    }
                }
                
                // we dont want both sides of a wall snapping to the same point (a problem with small walls) so we exclude that first postion from the check
                if(closestDist < acc && closest != exclude){
                    return closest;
                }
                return point;
            };
            
            if(mapCanv.mouseOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && drawingWall == false){
                startPos = c(mapCanv.getMouseOverPixel(window)) + playerPos - c(MiniMapX/2, MiniMapY/2);
                
                // L-shift will disable snapping the positions
                if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    startPos = getClosestPoint(startPos, sf::Vector2f(-1, -1));
                drawingWall = true;
            }
            
            if(mapCanv.mouseOver && !sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && drawingWall == true){
                drawingWall = false;
                sf::Vector2f endPos = c(mapCanv.getMouseOverPixel(window)) + playerPos - c(MiniMapX/2, MiniMapY/2);
                
                // L-shift will disable snapping the positions
                if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    endPos = getClosestPoint(endPos, startPos);
                
                // "-1" means that the wall will assign itself a random tag
                walls.push_back(Wall("-1", startPos, endPos, materials[selectedMaterialIndex]));
                
                
                //Send this info to the server (if connected)
                sf::Packet wallData;
                
                wallData << "newWall" << walls[walls.size()-1].tag << startPos.x << startPos.y << endPos.x << endPos.y << materials[selectedMaterialIndex].name;
                send(wallData);
                
                
            }
            
        }
        
        
        
        
        //===============
        // DELETING WALLS
        //===============
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && veiwCanvas.mouseOver && canDelWall)
            {
                canDelWall = false;
                if(results[veiwCanvas.getMouseOverPixel(window).x].hit)
                {
                    // get the tag, from the result, and remove the wall
                    removeWallByTag(walls, results[veiwCanvas.getMouseOverPixel(window).x].hits[0].wall->tag);
                    
                    // send this onfo to the server (if connected)
                    sf::Packet data;
                    data << "removeWall" << results[veiwCanvas.getMouseOverPixel(window).x].hits[0].wall->tag;
                    send(data);
                }
            }
            if(!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                canDelWall = true;
            
            if(mapCanv.mouseOver && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                playerPos = c(mapCanv.getMouseOverPixel(window));
            }
        }
        
        
        
        
        
        
        //===============
        // Get ray casting results
        //===============
        results.clear();
        //This is the amount of radians we want to inrement each ray (relative to 90 degrees or PI/2 radians)
        float incrementAmount = (PI/(180.0)*FOV/(float)ResX);
        
        //We want 0 radians (relative to veiw angle) to be in the middle se we start halfway back
        for(int a = -(ResX/2.0); a < ResX/2.0; a++){
            results.push_back(cast(playerPos, playerAngle, a*incrementAmount, walls));
        }
        backRay = cast(playerPos, playerAngle, PI, walls);
        
        
        
        //If any of the material buttons were pressed note that in variables
        for(int i = 0; i < materialButtons.size(); i++)
        {
            if(materialButtons[i].clickedOnTick)
            {
                selectedMaterialIndex = i;
            }
        }
        
        
        
        
        //=================
        // INPUT
        //=================
        if(sf::Mouse::getPosition(window).x > 0 && sf::Mouse::getPosition(window).x < window.getSize().x &&
           sf::Mouse::getPosition(window).y > 0 && sf::Mouse::getPosition(window).y < window.getSize().y ){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                playerAngle+=3.5*dt;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                playerAngle-=3.5*dt;
            
            // if theres a wall directly in front of us we dont want to move farward
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
                bool move = true;
                if(results[(int)(results.size()/2)].hit)
                    if(results[(int)(results.size()/2)].hits[0].dist < 5 && !results[(int)(results.size()/2)].hits[0].wall->passThrough)
                        move = false;
                
                if(move){
                    playerLoadedStep.x += dt*cos(playerAngle)*50;
                    playerLoadedStep.y += dt*sin(playerAngle)*50;
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                bool move = true;
                if(backRay.hit)
                    if(backRay.hits[0].dist < 5 && !backRay.hits[0].wall->passThrough)
                        move = false;
                
                if(move){
                    playerLoadedStep.x -= dt*cos(playerAngle)*50;
                    playerLoadedStep.y -= dt*sin(playerAngle)*50;
                }
            }
            if(abs(playerLoadedStep.x) > 1){
                playerPos.x +=  round(playerLoadedStep.x);
                playerLoadedStep.x = 0;
            }
            if(abs(playerLoadedStep.y) > 1){
                playerPos.y += round(playerLoadedStep.y);
                playerLoadedStep.y = 0;
            }
        }
        
        
        
        
        
        //================
        // RENDERING
        //================
        
        veiwCanvas.clear();
        mapCanv.clear();
        
        // Draw mini map canvas
        
        // Draw walls on mini map
        mapCanv.brushColor = sf::Color::White;
        for(int i = 0; i < walls.size(); i++){
            mapCanv.drawLine(walls[i].pos1 - playerPos + c(MiniMapX/2, MiniMapY/2), walls[i].pos2 - playerPos + c(MiniMapX/2, MiniMapY/2));
        }
        
        
        for(int i = 0; i < results.size(); i++){
            
            mapCanv.brushColor = sf::Color::White;
            if(veiwCanvas.getMouseOverPixel(window).x > i-5 && veiwCanvas.getMouseOverPixel(window).x < i+5){
                mapCanv.brushColor = sf::Color::Red;
            }
            
            //If the ray hit something we want to draw the ray
            if(results[i].hit){
                mapCanv.drawLine(c(MiniMapX/2, MiniMapY/2), c(results[i].hits[0].posI) - playerPos + c(MiniMapX/2, MiniMapY/2));
                
                // For all other hits (through wall) we want to draw a green dot
                mapCanv.brushColor = sf::Color::Green;
                for(int a = 1; a < results[i].hits.size(); a++){
                    mapCanv.drawPixel(results[i].hits[a].posF - playerPos + c(MiniMapX/2, MiniMapY/2));
                }
                
            }
            
        }
        mapCanv.brushColor = sf::Color::Green;
        mapCanv.drawLine(c(MiniMapX/2, MiniMapY/2), c(cos(playerAngle)*20+MiniMapX/2, sin(playerAngle)*20+MiniMapY/2));
        
        mapCanv.brushColor = sf::Color::Red;
        mapCanv.fillCircle(c(MiniMapX/2, MiniMapY/2), 4);
        
        
        
        // draw main veiw canvas
        
        std::vector<sf::Color> lastSlice;
        int lastStartY = 0;
        for(int i = 0; i < results.size(); i++){
            
            //veiwCanvas.brushColor = sf::Color::White;
            std::vector<Hit> hits = results[i].hits;
            
            
            // If the ray didnt hit anything, check if another one did and try to fill the (small) gap
            bool fillable = false;
            
            if(!results[i].hit && i > 0)
            {
                if(results[i-1].hit){
                    // Loop through the next 10 rays...
                    for(int a = i; a < i+10 && a < results.size(); a++)
                    {
                        // if any rays hit then e want to fill the gap
                        if(results[a].hit)
                            fillable=true;
                    }
                    
                    if(fillable){
                        // set to true so that the other empty gaps get filled
                        results[i].hit = true;
                        for(int p = 0; p < lastSlice.size(); p++)
                        {
                            veiwCanvas.brushColor = lastSlice[p];
                            veiwCanvas.drawPixel(c(i, lastStartY+p));
                        }
                    }
                }
            }
            
            
            //if(i==veiwCanvas.getMouseOverPixel(window).x)
            //  std::cout << fillable << "\n";
            
            
            if(results[i].hit){
                for(int h = (int)hits.size()-1; h >= 0; h--){
                    
                    float wallLength = sqrt(pow(hits[h].wall->pos2.x-hits[h].wall->pos1.x,2) + pow(hits[h].wall->pos2.y-hits[h].wall->pos1.y,2));
                    float unitsAccross = sqrt(pow((hits[h].posF.x-hits[h].wall->pos1.x),2) + pow((hits[h].posF.y-hits[h].wall->pos1.y),2));
                    float wallProg = unitsAccross/wallLength;
                    
                    float height = wallHeight*200/hits[h].trueDist;
                    float heightOffset = slider.getValue()*200/hits[h].trueDist;
                    
                    
                    // Note: this will go outside the bounds of the canvas, but the canvas can handle it, other implementations of a canvas may not
                    // May cause lag
                    lastSlice.clear();
                    lastStartY = (ResY/2-height/2);
                    
                    sf::Image *texture = &hits[h].wall->material.image;
                    // std::cout << "name: " <<  hits[h].wall->tag << "\n";
                    for(int a = 0; a < height; a++){
                        // maybe put a check outside the loop to prevent this check being run reduntantly every pixel
                        if((ResY/2-height/2)+a+heightOffset > 0 && (ResY/2-height/2)+a+heightOffset < ResY){
                            veiwCanvas.brushColor = texture->getPixel(sf::Vector2u((int)(texture->getSize().x*wallProg), (a/height*texture->getSize().y)));
                            //veiwCanvas.brushColor = sf::Color::White;
                            lastSlice.push_back(veiwCanvas.brushColor);
                            veiwCanvas.drawPixel(c(i, (ResY/2-height/2)+a+heightOffset));
                        }
                    }
                }
            }
        }
        
        total += getDT();
        msTmout -= 1;
        if(msTmout == 0)
        {
            msTmout = 10;
            //std::cout << 1/(total/10) << " FPS\n";
            total = 0;
        }
        //
        window.clear();
        window.draw(background);
        widgetMan.renderStack(window);
        window.display();
        
        materialWindow.clear();
        materialWidgetMan.renderStack(materialWindow);
        materialWindow.display();
        
        
    }
    //*/
}
