<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Hydroponic System Monitoring</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f4f8;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            flex-direction: column;
        }

        header {
            background-color: #007bff;
            color: white;
            padding: 20px 0;
            width: 100%;
            text-align: center;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        }

        h1 {
            margin: 0;
            font-size: 2rem;
        }

        table {
            border-collapse: collapse;
            width: 90%;
            max-width: 1200px;
            margin: 20px auto;
            background-color: white;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }

        th, td {
            padding: 12px 15px;
            text-align: center;
            border-bottom: 1px solid #ddd;
        }

        th {
            background-color: #007bff;
            color: white;
        }

        tr:hover {
            background-color: #f1f1f1;
        }

        footer {
            margin-top: 20px;
            text-align: center;
            color: #888;
            font-size: 0.9rem;
        }

        .info {
            margin: 20px 0;
            font-size: 1.2rem;
            color: #555;
        }

    </style>
</head>
<body>
    <header>
        <h1>Hydroponic System Monitoring</h1>
    </header>

    <div class="info">
        Below is the latest monitoring data fetched from the database:
    </div>

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
    }

    $sql = "SELECT * FROM tbl_hidroponik ORDER BY timestamp DESC";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        echo "<table border='1'>";
        echo "<tr><th>pH Level</th><th>Solenoid</th><th>Motor</th><th>Timestamp</th></tr>";
        while ($row = $result->fetch_assoc()) {
            echo "<tr>
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

    <footer>
        &copy; 2024 Hydroponic Monitoring System. All rights reserved.
    </footer>
</body>
</html>
