// drone_dashboard.ts
interface DroneData {
    x: number;
    y: number;
    trashDetected: boolean;
    category?: string;
}

// Simulate drone sending data every second
function simulateDroneData(callback: (data: DroneData) => void) {
    setInterval(() => {
        const data: DroneData = {
            x: Math.random() * 100,
            y: Math.random() * 100,
            trashDetected: Math.random() > 0.5,
            category: ['plastic', 'metal', 'organic'][Math.floor(Math.random() * 3)]
        };
        callback(data);
    }, 1000);
}

// Display data
simulateDroneData((data) => {
    console.log(`Drone at (${data.x.toFixed(2)}, ${data.y.toFixed(2)}), Trash: ${data.trashDetected ? data.category : 'None'}`);
});
