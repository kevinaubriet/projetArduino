// datamController.js
// Import datam model
Datam = require("./dataModel");
// Handle index actions
exports.index = function(req, res) {
  Datam.get(function(err, datams) {
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
  var datam = new Datam();
  datam.valeur = req.body.valeur;
  //datam.time = req.body.time;
  // save the datam and check for errors
  datam.save(function(err) {
    // if (err)
    //     res.json(err);
    res.json({
      message: "New datam created!" + " truc crée : " + req.body.valeur,
      data: datam
    });
  });
};
// Handle view datam info
exports.view = function(req, res) {
  Datam.findById(req.params.datam_id, function(err, datam) {
    if (err) res.send(err);
    res.json({
      message: "Datam details loading..",
      data: datam
    });
  });
};
// Handle update datam info
exports.update = function(req, res) {
  Datam.findById(req.params.datam_id, function(err, datam) {
    if (err) res.send(err);
    datam.valeur = req.body.valeur;
    //datam.time = req.body.time;
    // save the datam and check for errors
    datam.save(function(err) {
      if (err) res.json(err);
      res.json({
        message: "Datam Info updated",
        data: datam
      });
    });
  });
};
// Handle delete datam
exports.delete = function(req, res) {
  Datam.remove(
    {
      _id: req.params.datam_id
    },
    function(err, datam) {
      if (err) res.send(err);
      res.json({
        status: "success",
        message: "Datam deleted"
      });
    }
  );
};
