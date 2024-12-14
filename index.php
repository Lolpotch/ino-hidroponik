<?php
include_once 'koneksi.php';

date_default_timezone_set('Asia/Jakarta');

// Check if 'data_ph' is provided in the URL
if (isset($_GET['data_ph']) && is_numeric($_GET['data_ph'])) {
    $data_ph = $_GET['data_ph'];
    $current_time = date('H:i');

    $solenoid_status = ($current_time >= '10:00' && $current_time <= '14:00') ? 'ON' : 'OFF';
    $motor_status = ($data_ph > 6) ? 'ON' : 'OFF';

    $sql = "INSERT INTO tbl_hidroponik (data_ph, solenoid_status, motor_status)    
            VALUES ('$data_ph', '$solenoid_status', '$motor_status')";

    if ($conn->query($sql) === TRUE) {
        echo "New record created successfully.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    ob_end_flush(); // Mengakhiri buffering
    exit;
}

$sql = "SELECT * FROM tbl_hidroponik ORDER BY timestamp DESC";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "<table border='1'>";
    echo "<tr><th>ID</th><th>pH Level</th><th>Solenoid</th><th>Motor</th><th>Timestamp</th></tr>";
    while ($row = $result->fetch_assoc()) {
        echo "<tr>
                <td>{$row['id']}</td>
                <td>{$row['data_ph']}</td>
                <td>{$row['solenoid_status']}</td>
                <td>{$row['motor_status']}</td>
                <td>{$row['timestamp']}</td>
              </tr>";
    }
    echo "</table>";
} else {
    echo "No data found.";
}

$conn->close();
?>