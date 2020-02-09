// User values

materialThiccnes = 4; // mm
frontMaterialThiccnes = 3; //mm
boltSize = 3; //mm

backheight = 30;  // mm
frontheight = 10; // mm

ledSize = 6;      // mm
ledOffset = 250 / 15;   // mm
amountOfLeds = 16; // number

margin = 5; // mm

slotsAmount = 20;
sideSlotsAmount = 2;

createOnlyBack = true;

partSpacing = 1; //mm

// Computer caclulated values

ledSpacing = ledOffset - ledSize;

amountOfSpacers = amountOfLeds + 1;
spacerNegativeWidth = ledOffset - materialThiccnes;
spacerOffset = ledSpacing / 2 - materialThiccnes / 2;

fullMargin = materialThiccnes + margin + (spacerOffset + materialThiccnes);

baseSquareSize = (fullMargin * 2) + (amountOfLeds * ledSize) +
                 ((amountOfLeds - 1) * ledSpacing);

baseSquareCenter = [ baseSquareSize / 2, baseSquareSize / 2, 0 ];

boxHeight = materialThiccnes + backheight + materialThiccnes + frontheight +
            materialThiccnes;

completeSlots = slotsAmount * 2 + 1;
slotLength = baseSquareSize / completeSlots;

completeSideSlots = sideSlotsAmount * 2 + 1;
sideSlotLength = boxHeight / completeSideSlots;

echo("Complete width: ", baseSquareSize);
echo("Front width: ", baseSquareSize - materialThiccnes * 2);
echo("Amount of spacers: ", amountOfSpacers);

// Functions

function isEven(num) = (num % 2 == 0) ? 1 : 0;

// Modules

module createRect(x, y, width, height)
{
    translate([ x, y, 0 ]) { square([ width, height ], center = false); }
}

module createSquare(x, y, size)
{
    createRect(x, y, size, size);
}

module createCircle(x, y, diameter, quality = 360){
    radius = diameter / 2;
    translate([x + radius, y + radius, 1])
    {
        circle(d=diameter, $fn=quality);
    }
}

// Specific modules

module
createSpacerSquareRow()
{
    for (i = [0:amountOfSpacers - 1]) {
        x = i * ledOffset + fullMargin - ledSpacing + spacerOffset;
        createSquare(x, 0, materialThiccnes);
    }
}

module createSlots(slotOffset = 0)
{
    for (i = [0:completeSlots - 1]) {
        if (i % 2 == slotOffset) {
            rectOffset = i * slotLength;

            createRect(rectOffset, 0, slotLength, materialThiccnes);
        }
    }
}

module
createFrontAndBack()
{
    i = 0;
    
    currentOffset = (baseSquareSize + partSpacing) * (i + 1);
    translate([ 0, currentOffset, 0 ]) { 
        createMain(false, false); 
    }
    
    if(!createOnlyBack){
        i = i + 1;
        currentOffset = (baseSquareSize + partSpacing) * (i + 1);
        translate([ 0, currentOffset, 0 ]) { 
            createMain(false, false); 
        }
    }
}

module
createMain(slots = true, large = true)
{
    
    module createMainBoltHoles(){
        boltHoleOffset = materialThiccnes + (margin / 2) - (boltSize - 2);
        union(){
            createCircle(boltHoleOffset, boltHoleOffset, boltSize, 20);
            createCircle(
                baseSquareSize - boltHoleOffset - boltSize, 
                boltHoleOffset, 
                boltSize, 20);
            createCircle(
                boltHoleOffset, 
                baseSquareSize - boltHoleOffset - boltSize, 
                boltSize, 20);
            createCircle(
                baseSquareSize - boltHoleOffset - boltSize, 
                baseSquareSize - boltHoleOffset - boltSize, 
                boltSize, 20);
        }
    }
    
    module createAllMainSlots()
    {
        createSlots();
        translate([ 0, baseSquareSize - materialThiccnes, 0 ])
        {
            createSlots();
        }

        translate([ materialThiccnes, 0, 0 ]) rotate([ 0, 0, 90 ])
        {
            createSlots();
        }

        translate([ baseSquareSize, 0, 0 ]) rotate([ 0, 0, 90 ])
        {
            createSlots();
        }
    }
      
    difference()
    {
        if(large){
            createSquare(0, 0, baseSquareSize);
        }else{
            createSquare(
                materialThiccnes, materialThiccnes, 
                baseSquareSize - materialThiccnes * 2);
        }
        if(slots){
            createAllMainSlots();
        }
        createMainBoltHoles();
    }
}

module
createMainWithHoles()
{
    module createMainSquares()
    {
        for (i = [0:amountOfLeds - 1]) {
            for (j = [0:amountOfLeds - 1]) {
                // echo(i, j)
                x = i * ledOffset + fullMargin;
                y = j * ledOffset + fullMargin;
                createSquare(x, y, ledSize);
            }
        }
    }

    module createSpacerSquares()
    {
        for (i = [0:amountOfSpacers - 1]) {
            y = i * ledOffset + fullMargin - ledSpacing + spacerOffset;

            translate([ 0, y, 0 ]) { createSpacerSquareRow(); }
        }
    }

    difference()
    {
        createMain();
        createMainSquares();
        createSpacerSquares();
    }
}

module
createSidePanels()
{
    module createSidePanel(slotOffset = 0)
    {
        module createSidePanelSlots(slotOffset = 0)
        {
            for (i = [0:completeSideSlots - 1]) {
                if (i % 2 == slotOffset) {
                    rectOffset = i * sideSlotLength;

                    createRect(0, rectOffset, materialThiccnes, sideSlotLength);
                }
            }
        }
        
        module createSidePanelBackSlots(){
            union(){
                createRect(slotLength, 0, slotLength, materialThiccnes);
                createRect(baseSquareSize - slotLength * 2, 0, 
                    slotLength, materialThiccnes);
            }
        }

        width = baseSquareSize - materialThiccnes * 2;
        centerPlaneLocation = materialThiccnes + backheight;
        spacerHoleLocation = boxHeight - materialThiccnes - frontheight / 2 -
                             materialThiccnes / 2;

        difference()
        {
            createRect(0, 0, 
                baseSquareSize, boxHeight);

            translate([ 0, centerPlaneLocation, 0 ]) { createSlots(1); }

            translate([ 0, spacerHoleLocation, 0 ]) { createSpacerSquareRow(); }

            createSidePanelSlots(slotOffset);
            translate([ baseSquareSize - materialThiccnes, 0, 0 ])
            {
                createSidePanelSlots(slotOffset);
            }
            
            translate([0, materialThiccnes, 0]){
                createSidePanelBackSlots();
            }
        }
    }

    for (i = [0:3]) {
        sidePanelOffset = -1 * ((boxHeight + partSpacing) * (i + 1));
        translate([ 0, sidePanelOffset, 0 ]) { createSidePanel(isEven(i)); }
    }
}

module
createAllSpacers()
{

    module createSpacer(bottom = true)
    {

        module createSpacerOpenings(bottom = true)
        {
            openingY = bottom ? 0 : frontheight / 2;

            for (i = [0:amountOfSpacers - 1]) {
                x = i * ledOffset + fullMargin - ledSpacing + spacerOffset;
                createRect(x, openingY, materialThiccnes, frontheight / 2);
            }
        }

        spacerWidth = baseSquareSize - materialThiccnes * 2;

        squareY = (frontheight / 2) - materialThiccnes / 2;
        union()
        {
            translate([0, materialThiccnes, 0]){
                difference()
                {
                    createRect(materialThiccnes, 0, spacerWidth, frontheight);
                    createSpacerOpenings(bottom);
                }
                createSquare(0, squareY, materialThiccnes);
                createSquare(
                    baseSquareSize - materialThiccnes, 
                    squareY, 
                    materialThiccnes);
            }
            if(!bottom){
                createSpacerSquareRow();
            }
        }
    }

    for (i = [0:amountOfSpacers * 2 - 1]) {
       
        
        sidePanelOffset = -1 * 
        ((frontheight + materialThiccnes + partSpacing) * (i + 1))
        + (((i + 1) - ((i + 1) % 2)) / 2)  * materialThiccnes;

        translate([ - baseSquareSize - partSpacing, sidePanelOffset, 0 ])
        {
            createSpacer(isEven(i));
        }
    }
}

module createBackHolders(){
    backHolderSize = slotLength * 2 - materialThiccnes;
    boltHoleOffset = materialThiccnes + (margin / 2) - (boltSize - 2);
    
    module createOneBackHolder(){
        difference(){
            union(){
                createSquare(materialThiccnes, materialThiccnes, backHolderSize);
                createRect(slotLength, 0, slotLength, materialThiccnes);
                createRect(0, slotLength, materialThiccnes, slotLength);
            }
            createCircle(boltHoleOffset, boltHoleOffset, boltSize, 20);
        }
    }
    

    for(i = [1:4]){
        translate([-(backHolderSize + materialThiccnes + partSpacing) * i, 0, 0]){
            createOneBackHolder();
        }
    }
    
}

// Main code
createFrontAndBack();

createMainWithHoles();
createSidePanels();

createAllSpacers();

createBackHolders();




















