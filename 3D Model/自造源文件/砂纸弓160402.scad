$fn = 200;

module board() { //搓衣板
    union() {
        translate([-2, -13]) cube([4, 16, 20]);
        for(y = [0 : 5 : 10]) { //6根搓衣柱
            translate([-2, -y]) cylinder(20, 1, 1);
            translate([2, -y]) cylinder(20, 1, 1);
        }
        translate([0, 3]) cylinder(20, 2, 2);
        translate([0, -13]) cylinder(20, 2, 2);
    }
}

module curve() { //弓反曲
    translate([-45, 0, 0]) union() {
        intersection() {
            difference() {
                cylinder(20, 20, 20);
                translate([0, 0, -1]) cylinder(22, 15, 15);
            }
            translate([-20, 0]) cube([20, 20, 20]);
        }
        translate([-20, -17.5]) cube([5, 17.5, 20]);
        translate([-17.5, -17.5]) cylinder(20, 2.5, 2.5);
        translate([-20, -20]) cube([2.5, 5, 20]);
        translate([-29.2, -8.8]) rotate(45) board();
    }
}

curve(); //画一边反曲
mirror([1,0,0]) curve(); //YZ平面镜象
translate([-45, 15]) cube([90, 5, 20]); //中间连接