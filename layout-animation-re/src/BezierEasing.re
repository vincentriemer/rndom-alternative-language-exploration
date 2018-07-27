let newtonIterations = 4;
let newtonMinSlope = 0.001;
let subdivisionPrecision = 0.0000001;
let subdivisionMaxIterations = 10;

let kSplineTableSize = 11;
let kSampleStepSize = 1.0 /. (float_of_int(kSplineTableSize) -. 1.0);

let a = (aA1, aA2) => 1.0 -. 3.0 *. aA2 +. 3.0 *. aA1;
let b = (aA1, aA2) => 3.0 *. aA2 -. 6.0 *. aA1;
let c = aA1 => 3.0 *. aA1;

let calcBezier = (aT, aA1, aA2) =>
  ((a(aA1, aA2) *. aT +. b(aA1, aA2)) *. aT +. c(aA1)) *. aT;

let getSlope = (aT, aA1, aA2) =>
  3.0 *. a(aA1, aA2) *. aT *. aT +. 2.0 *. b(aA1, aA2) *. aT +. c(aA1);

let rec binarySubdivide =
        (~currentT=0.0, ~currentX=0.0, ~i=0, aX, aA, aB, mX1, mX2) =>
  switch (
    abs_float(currentX) > subdivisionPrecision
    && i
    + 1 < subdivisionMaxIterations
  ) {
  | false => currentT
  | true =>
    let currentT = aA +. (aB -. aA) /. 2.0;
    let currentX = calcBezier(currentT, mX1, mX2) -. aX;
    let i = i + 1;
    if (currentX > 0.0) {
      binarySubdivide(~currentT, ~currentX, ~i, aX, aA, currentT, mX1, mX2);
    } else {
      binarySubdivide(~currentT, ~currentX, ~i, aX, currentT, aB, mX1, mX2);
    };
  };

let rec newtonRaphsonIterate = (~i=0, aX, aGuessT, mX1, mX2) =>
  switch (i < newtonIterations) {
  | false => aGuessT
  | true =>
    let currentSlope = getSlope(aGuessT, mX1, mX2);
    switch (currentSlope) {
    | 0.0 => aGuessT
    | currentSlope =>
      let currentX = calcBezier(aGuessT, mX1, mX2) -. aX;
      let aGuessT = aGuessT -. currentX /. currentSlope;
      let i = i + 1;
      newtonRaphsonIterate(~i, aX, aGuessT, mX1, mX2);
    };
  };

let linearEasing = x => x;

let bezier = (mX1, mY1, mX2, mY2) =>
  ! (0. <= mX1 && mX1 <= 1. && 0. <= mX2 && mX2 <= 1.) ?
    failwith("bezier x values must be in [0, 1] range") :
    mX1 == mY1 && mX2 == mY2 ?
      linearEasing :
      {
        let sampleValues =
          Array.init(kSplineTableSize, i =>
            calcBezier(float_of_int(i) *. kSampleStepSize, mX1, mX2)
          );

        let getTForX = aX => {
          let intervalStart = ref(0.0);
          let currentSample = ref(1);
          let lastSample = kSplineTableSize - 1;

          while (currentSample^ !== lastSample
                 && sampleValues[currentSample^] <= aX) {
            intervalStart := intervalStart^ +. kSampleStepSize;
            currentSample := currentSample^ + 1;
          };

          currentSample := currentSample^ - 1;

          /* Interpolate to provide an initial guess for t */
          let dist =
            (aX -. sampleValues[currentSample^])
            /. (
              sampleValues[currentSample^ + 1] -. sampleValues[currentSample^]
            );
          let guessForT = intervalStart^ +. dist *. kSampleStepSize;

          let initialSlope = getSlope(guessForT, mX1, mX2);
          switch (initialSlope >= newtonMinSlope, initialSlope == 0.0) {
          | (true, _) => newtonRaphsonIterate(aX, guessForT, mX1, mX2)
          | (_, true) => guessForT
          | (_, _) =>
            binarySubdivide(
              aX,
              intervalStart^,
              intervalStart^ +. kSampleStepSize,
              mX1,
              mX2,
            )
          };
        };

        x =>
          switch (x) {
          | 0.0 => 0.0
          | 1.0 => 1.0
          | x => calcBezier(getTForX(x), mY1, mY2)
          };
      };