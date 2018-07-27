const KeyframeGenerator = require("./KeyframeGenerator.bs");

function generateKeyframes(easeType, delay, duration) {
  const rawResult = KeyframeGenerator.generateKeyframes(
    easeType,
    delay,
    duration
  );
  return KeyframeGenerator.keyframeResultToJs(rawResult);
}

module.exports.generateKeyframes = generateKeyframes;

module.exports.EasingType = {
  linear: KeyframeGenerator.linear,
  easeIn: KeyframeGenerator.easeIn,
  easeOut: KeyframeGenerator.easeOut,
  easeInEaseOut: KeyframeGenerator.easeInEaseOut
};
