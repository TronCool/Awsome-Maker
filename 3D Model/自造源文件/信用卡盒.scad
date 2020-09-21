//壁厚2MM 35个卡位，间距1.5mm
$fn = 100;

translate([2, 2, 0]) difference() {
    translate([1, 1, 2]) minkowski() {
        cube([140, 88, 28]);
        sphere(2);
    }
    translate([1, 1, 2]) cube([140, 88, 32]);
    translate([-1, -1, 31.8]) cube([144, 94, 5]);
}


difference() {
    for (a =[4.5:2:70.5]) 
        translate([a, 2, 2]) rotate([0,20,0]) cube([.5, 90, 25]);
    translate([4, 7, 2]) cube([130, 80, 36]);
}