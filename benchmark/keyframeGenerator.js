const Benchmark = require("benchmark");

const Reason = require("../layout-animation-re/src/index");
const Js = require("../layout-animation-js/lib/KeyframeGenerator");

const suite = new Benchmark.Suite({ name: "KeyframeGenerator" });

suite
  .add("Reason", function() {
    Reason.generateKeyframes(Reason.EasingType.linear, 0, 1000);
    Reason.generateKeyframes(Reason.EasingType.easeIn, 0, 1000);
    Reason.generateKeyframes(Reason.EasingType.easeOut, 0, 1000);
    Reason.generateKeyframes(Reason.EasingType.easeInEaseOut, 0, 1000);
  })
  .add("JS", function() {
    Js.generateKeyframes(Js.EasingType.linear, 0, 1000);
    Js.generateKeyframes(Js.EasingType.easeIn, 0, 1000);
    Js.generateKeyframes(Js.EasingType.easeOut, 0, 1000);
    Js.generateKeyframes(Js.EasingType.easeInEaseOut, 0, 1000);
  })
  .on("error", console.error)
  .on("cycle", function(event) {
    console.log();
    console.log(String(event.target));
  })
  .on("complete", function() {
    console.log();
    console.log("Fastest is " + this.filter("fastest").map("name"));
  })
  .run();
