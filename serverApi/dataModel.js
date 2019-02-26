// dataModel.js
var mongoose = require("mongoose");
mongoose.connect("mongodb://localhost/arduino");
// Setup schema
var dataSchema = mongoose.Schema({
  valeur: {
    type: String,
    required: true
  },
  time: {
    type: Date,
    default: Date.now
  }
});
// Export Contact model
var Datam = (module.exports = mongoose.model("temperatures", dataSchema));
module.exports.get = function(callback, limit) {
  Datam.find(callback).limit(limit);
};
