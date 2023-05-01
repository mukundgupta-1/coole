const express = require('express');
const bodyParser = require('body-parser');
const rosnodejs = require('rosnodejs');
const websocketController = require('./controllers/websocketController');

const app = express();

// Body Parser Middleware
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

// Serve static files from the public directory
app.use(express.static('public'));

// Websocket connection
const wsController = new websocketController();
const websocket = require('express-ws')(app);

app.ws('/ws', (ws, req) => {
  wsController.handleConnection(ws);
});

// Get items API endpoint
app.get('/api/items', (req, res) => {
  // Replace with your logic for getting the available items from your inventory
  const items = ['pen', 'medicines', 'bottle'];
  res.json(items);
});

// Get beds API endpoint
app.get('/api/beds', (req, res) => {
  // Replace with your logic for getting the available beds
  const beds = ['bed1', 'bed2', 'bed3', 'bed4'];
  res.json(beds);
});

// Send order API endpoint
app.post('/api/order', async (req, res) => {
  const { bed, items } = req.body;

  // Connect to ROS
  const rosNode = await rosnodejs.initNode('web_app');
  const pub = rosNode.advertise('/order', 'std_msgs/String');

  // Publish the order message
  const msg = new rosnodejs.std_msgs.String();
  msg.data = JSON.stringify({ bed, items });
  pub.publish(msg);

  res.json({ success: true });
});

// Start server
const PORT = process.env.PORT || 5000;
app.listen(PORT, () => console.log(`Server started on port ${PORT}`));
