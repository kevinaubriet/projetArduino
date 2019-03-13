// datamController.js
// Import datam model
LedM = require("./ledModel");
let router = require("express").Router();

// Handle index actions
exports.index = function(req, res) {
  LedM.get(function(err, datams) {
    if (err) {
      res.json({
        status: "error",
        message: err
      });
    }
    res.json({
      status: "success",
      message: "Datams retrieved successfully",
      data: datams
    });
  });
};
// Handle create datam actions
exports.new = function(req, res) {
  var datam = new LedM();
  datam.id = 1;
  datam.valeur = false; //req.body.valeur;
  // save the datam and check for errors
  datam.save(function(err) {
    // if (err)
    //     res.json(err);
    res.json({
      message: "New datam created!" + " valeur : " + datam.valeur,
      data: datam
    });
  });
};
// Handle view datam info
exports.view = function(req, res) {
  LedM.findById(req.params.datam_id, function(err, datam) {
    if (err) res.send(err);
    res.json({
      message: "Datam details loading..",
      data: datam
    });
  });
};
// Handle update datam info
exports.update = function(req, res) {
  if (!LedM.findById(req.params.datam_id)) {
    console.log("existe");
  } else {
    console.log("existe pas");
  }

  LedM.findById(req.params.datam_id, function(err, datam) {
    if (err) {
      //router.route("/led").post(ledController.new);
      //res.send(err);
      datam.valeur = false;
      // save the datam and check for errors
      datam.save(function(err) {
        if (err) res.json(err);
        res.json({
          message: "Datam Info updated",
          data: datam
        });
      });
    } else {
      datam.valeur = req.body.valeur;
      // save the datam and check for errors
      datam.save(function(err) {
        if (err) res.json(err);
        res.json({
          message: "Datam Info updated",
          data: datam
        });
      });
    }
  });
};
// Handle delete datam
exports.delete = function(req, res) {
  LedM.remove(
    {
      _id: req.params.datam_id
    },
    function(err, datam) {
      if (err) res.send(err);
      res.json({
        status: "success",
        message: "Datam deleted "
      });
    }
  );
};
