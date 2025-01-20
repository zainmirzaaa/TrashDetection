// drone_dashboard.ts
import * as fs from 'fs';

interface DroneData {
    x: number;
    y: number;
    trashDetected: boolean;
    category?: string;
}

// Store drone data in a batch
const droneDataBatch: DroneData[] = [];

// Save current batch to a JSON file
function saveDroneData(filename: string = 'drone_detections.json') {
    fs.writeFile(filename, JSON.stringify(droneDataBatch, null, 2), (err) => {
        if (err) console.error("Error saving drone data:", err);
        else console.log(`Drone data saved to ${filename}`);
    });
}

// Simulate drone sending data every second and store in batch
function simulateDroneData(callback: (data: DroneData) => void) {
    setInterval(() => {
        const data: DroneData = {
            x: Math.random() * 100,
            y: Math.random() * 100,
            trashDetected: Math.random() > 0.5,
            category: ['plastic', 'metal', 'organic'][Math.floor(Math.random() * 3)]
        };
        droneDataBatch.push(data);
        callback(data);
    }, 1000);
}

// Filter drone data for a specific trash category
function filterByCategory(category: string): DroneData[] {
    return droneDataBatch.filter(d => d.trashDetected && d.category === category);
}

// Run simulation
simulateDroneData((data) => {
    const status = data.trashDetected ? data.category : 'None';
    console.log(`[${new Date().toLocaleTimeString()}] Drone at (${data.x.toFixed(2)}, ${data.y.toFixed(2)}), Trash: ${status}`);
});

// Save drone data every 5 seconds
setInterval(() => {
    saveDroneData();
}, 5000);

// Example usage: log all detected plastics every 5 seconds
setInterval(() => {
    const plastics = filterByCategory('plastic');
    console.log(`[${new Date().toLocaleTimeString()}] Detected plastics count: ${plastics.length}`);
}, 5000);

// Optional: display total detections periodically
setInterval(() => {
    const total = droneDataBatch.length;
    console.log(`[${new Date().toLocaleTimeString()}] Total drone detections so far: ${total}`);
}, 10000);
