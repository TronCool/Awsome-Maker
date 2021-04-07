$fn = 200;
linear_extrude(height = 20) difference() {
    hull() {
        translate([ -15, 25, 0 ]) circle(d = 10);
        translate([ 10, 20, 0 ]) circle(d = 20);
        translate([ -5, -15, 0 ]) circle(d = 30);
        translate([ 0, -10, 0 ]) circle(d = 40);
    }
    hull() {
        translate([ -14.5, 24.5, 0 ]) circle(d = 1);
        translate([ 10, 20, 0 ]) circle(d = 10);
        translate([ -5, -15, 0 ]) circle(d = 20);
        translate([ 0, -10, 0 ]) circle(d = 30);
    }
}