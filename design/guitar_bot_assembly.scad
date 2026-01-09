/*
    Guitar Robot - Full Assembly Visualization
    Use 'barre_unit.scad' and 'bridge_plucker.scad' for printing.
*/

include <barre_unit.scad>
include <bridge_plucker.scad>

// Parameters need to match included files or be re-declared if those files are standalone.
// For simplicity, we assume the included files provide modules.

// --- Assembly ---

// Show 4 Fret Bars spaced out
echo("Rendering Assembly...");

for (i=[0:3]) {
    translate([0, i * 35, 0])
        BarreUnit(str(i+1));
}

// Show Bridge Unit (Way back)
translate([-20, -150, 0])
    BridgePluckerArray();
