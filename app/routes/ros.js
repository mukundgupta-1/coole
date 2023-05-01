const express = require('express');
const router = express.Router();
const rosnodejs = require('rosnodejs');
const { Twist } = rosnodejs.require('geometry_msgs').msg;
const { ROSLIB } = require('roslib');

// Set up ROS node
rosnodejs.initNode('/web_api')
  .then((rosNode) => {
    // Create publisher to send Twist messages to the robot
    const cmdVelPublisher = rosNode.advertise('/cmd_vel', Twist);

    // Create subscriber to receive distance updates from the robot
    const distanceSubscriber = rosNode.subscribe('/distance', 'std_msgs/Float32', (message) => {
      console.log('Received distance update:', message.data);
    });

    // Create a ROSLIB WebSocket to listen for messages from the web UI
    const ws = new ROSLIB.WebSocket('ws://localhost:9090');

    // When a message is received on the WebSocket, send a Twist message to the robot
    ws.on('message', (message) => {
      console.log('WebSocket message:', message);

      const orders = JSON.parse(message);

      // Sort orders by distance to perform priority scheduling
      orders.sort((a, b) => a.distance - b.distance);

      orders.forEach((order) => {
        // Send Twist message to robot
        const twist = new Twist({
          linear: { x: order.distance },
          angular: { z: order.angle },
        });
        cmdVelPublisher.publish(twist);

        // Send order confirmation to user
        const confirmation = new ROSLIB.Message({ data: `Delivering ${order.items} to bed ${order.bedNumber}` });
        const confirmationTopic = new ROSLIB.Topic({
          ros: rosNode,
          name: '/confirmation',
          messageType: 'std_msgs/String',
        });
        confirmationTopic.publish(confirmation);
      });
    });
  })
  .catch(console.error);

router.ws('/', (ws, req) => {
  console.log('WebSocket connected');

  ws.on('message', (message) => {
    console.log('WebSocket message:', message);

    // Send message to ROS node
    const order = JSON.parse(message);

    const topic = new ROSLIB.Topic({
      ros: rosNode,
      name: '/order',
      messageType: 'std_msgs/String',
    });

    const rosMessage = new ROSLIB.Message({ data: JSON.stringify(order) });
    topic.publish(rosMessage);
  });
});

module.exports = router;
