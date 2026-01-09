/*
    Bridge Plucker Array - Printable Part
    Print 1 copy of this file
*/

// --- Parameters ---
num_strings = 6;
string_spacing = 11; // Standard electric guitar spacing at bridge
servo_L = 23.5;
servo_W = 12.8;

module ServoMG90S_Cutout() {
    cube([servo_L, servo_W, 20]);
    // Cable hole
    translate([0, 0, -5]) cube([servo_L, 5, 20]);
}

module BridgePluckerArray() {
    total_w = num_strings * string_spacing + 20;
    total_d = servo_L + 15;
    total_h = 25;
    
    difference() {
        // Main Block
        translate([-total_w/2, 0, 0])
            cube([total_w, total_d, total_h]);
            
        // Servo Slots
        for (i = [0 : num_strings-1]) {
            // Calculate X pos relative to center
            x_pos = (i - (num_strings-1)/2) * string_spacing;
            
            translate([x_pos - servo_W/2, 5, 5])
                rotate([0,0,90]) // Rotate servos to face strings
                ServoMG90S_Cutout();
                
            // Actuator Arm Slot (Front)
            translate([x_pos - 4, -5, 10])
                cube([8, 15, 20]);
        }
        
        // Mounting Holes (To stick to guitar body)
        translate([-total_w/2 + 5, total_d/2, 0])
            cylinder(d=4, h=50, center=true, $fn=20);
        translate([total_w/2 - 5, total_d/2, 0])
            cylinder(d=4, h=50, center=true, $fn=20);
    }
}

// Render for printing
BridgePluckerArray();
