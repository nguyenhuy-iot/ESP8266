<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>send data</title>
</head>

<body>
    <?php
    $fileName = 'data.txt';
    // // echo '</pre>';   
    //ghi file
    if (isset($_POST["DATA"]) && $_POST["DATA"] != "") {
        //echo var_dump($_POST);
        $data = $_POST["DATA"];
        $data .= "\r\n"; //them xuong dong
        file_put_contents($fileName, $data, FILE_APPEND);
    }
    //doc file
    $file = @fopen('data.txt', 'r');
    if (!$file)
        echo "Mở file không thành công";
    else {
        while (!feof($file)) { // hàm feof sẽ trả về true nếu ở vị trí cuối cùng của file
            echo fgets($file);  // đọc ra từng dòng trong file
            echo "</br>";
        }
        fclose($file);
    }


    ?>

    <form action="#" method="post">
        <input type="text" name="DATA">
        <input type="submit" value="SEND">
    </form>

</body>

</html>