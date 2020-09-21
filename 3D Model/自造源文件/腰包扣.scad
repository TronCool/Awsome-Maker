$fn = 100;
color("gray") difference() { 
    minkowski() { 
        translate([3.8, 3.8, 0.8]) minkowski() { 
            cube([37.4, 7.4, 1.4]); 
            cylinder(h=0.4, r=3); 
        }
        sphere(0.8); 
    } 
    
    union() {
        translate([3.8, 3.8, -0.5]) minkowski() { 
            cube([37.4, 1.4, 4]); 
            cylinder(h=5, r=0.8); 
        } 
        translate([3.8, 9.8, -0.5]) minkowski() {
            cube([37.4, 1.4, 4]); 
            cylinder(h=5, r=0.8); 
        }
    }
    translate([22, -1, -1]) cube([1, 5, 5]); 
    translate([22, 11, -1]) cube([1, 5, 5]); 
}
