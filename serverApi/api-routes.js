// api-routes.js
// Initialize express router
let router = require("express").Router();
// Set default API response
router.get("/", function(req, res) {
  res.json({
    status: "API Its Working",
    message: "Welcome to RESTHub crafted with love!"
  });
});
// Import contact controller
var dataController = require("./data/dataController");
var ledController = require("./led/ledController");
// Contact routes
router
  .route("/datas")
  .get(dataController.index)
  .post(dataController.new);
router.route("/datas/:type").get(dataController.index);
router
  .route("/led")
  .get(ledController.index)
  .post(ledController.new);
router
  .route("/led/:datam_id")
  .patch(ledController.update)
  .put(ledController.update)
  .delete(ledController.delete);
// Export API routes
module.exports = router;
