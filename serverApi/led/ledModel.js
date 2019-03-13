// dataModel.js
var mongoose = require("mongoose");
mongoose.connect("mongodb://localhost/ledAubrietBouajla");
// Setup schema
var ledSchema = mongoose.Schema({
  id: {
    type: Number,
    required: true,
    default: 1
  },
  valeur: {
    type: Boolean,
    required: true
  },
  time: {
    type: Date,
    default: Date.now
  }
});
// Export Contact model
var Datam = (module.exports = mongoose.model("led", ledSchema));
module.exports.get = function(callback, limit) {
  Datam.find(callback).limit(limit);
};
