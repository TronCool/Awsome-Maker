$fn = 200;

difference(){
    intersection() {
        translate([-12,-7]) minkowski() {
            cube([24,14,10]);
            cylinder(r=3);
        }
        translate([-15,0]) cube([30,15,12]);
    }
    translate([-16,-3,12]) rotate([-25,0,0]) cube([32,15,12]);
}
intersection() {
    translate([-12,-7]) minkowski() {
        cube([24,14,2]);
        cylinder(r=3);
    }
    translate([-15,-15]) cube([30,15,12]);
}

translate([-13,-2]) cube([26,4,12]);
translate([-13, 0, 1]) cylinder(r=2, h=11);
translate([13, 0, 1]) cylinder(r=2, h=11);
translate([-13,0, 12]) rotate([0,90,0]) cylinder(r=2, h=26);
translate([-13,0, 12]) sphere(r=2);
translate([13,0, 12]) sphere(r=2);