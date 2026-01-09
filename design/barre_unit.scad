/*
    Barre Unit - Printable Part
    Print 4 copies of this file
*/

// --- Parameters ---
neck_width = 55;   // Width of guitar neck + padding
neck_height = 25;  // Thickness of guitar neck
servo_L = 23.5;    // Tolerance included
servo_W = 12.8;
servo_H = 23;

module ServoMG90S_Cutout() {
    cube([servo_L, servo_W, servo_H+10]);
    // Cable exit
    translate([servo_L-5, -5, 0]) cube([10, 20, 10]);
}

module BarreUnit(label="X") {
    base_w = neck_width + 30; 
    base_thick = 6;
    leg_width = 12;
    
    difference() {
        union() {
            // Main Arch
            translate([-base_w/2, 0, 0]) 
                cube([base_w, 20, base_thick]);
            
            // Left Leg
            translate([-base_w/2, 0, -neck_height]) 
                cube([leg_width, 20, neck_height]);
                
            // Right Leg + Servo Tower base
            translate([base_w/2 - leg_width, 0, -neck_height]) 
                cube([leg_width, 20, neck_height + 30]); // Taller for servo
                
            // Servo Tower Head
            translate([base_w/2 - leg_width - 5, 0, 15])
                cube([leg_width + 10, 20, 20]);
        }
        
        // Subtract Neck (Internal volume)
        translate([-base_w/2 + leg_width, -1, -neck_height-1])
            cube([base_w - 2*leg_width, 22, neck_height+2]);
            
        // Subtract Servo Hole (Side mounted)
        translate([base_w/2 - leg_width - 8, 3, 18])
            rotate([0, 0, 0])
            ServoMG90S_Cutout();
            
        // Mounting Screws for Clamp
        translate([-base_w/2 + leg_width/2, 10, -neck_height/2]) 
            rotate([0,90,0]) cylinder(d=3.5, h=base_w+10, center=true, $fn=20);
    }
    
    // Label
    color("White")
    translate([0, 8, base_thick])
        linear_extrude(1) text(label, size=8, halign="center");
}

// Render for printing
BarreUnit("1");
