body = """\

<!DOCTYPE html>
<html>
<head>
    <title>Web Server</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            text-align: center;
        }

        header {
            background-color: #333;
            color: #fff;
            padding: 20px;
        }

        h1 {
            font-size: 36px;
        }

        p {
            font-size: 18px;
            line-height: 1.5;
        }

        .container {
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
            background-color: #fff;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        .button {
            display: inline-block;
            padding: 10px 20px;
            background-color: #007BFF;
            color: #fff;
            text-decoration: none;
            border-radius: 5px;
        }
    </style>
</head>
<body>
    <header>
        <h1>Web Server</h1>
    </header>
    <div class="container">
        <h2>CGI</h2>
        <p>Aqui podrias poner lo que quieras, incluso podrias esditarlo y volvera ejecutarlo y se actualizaria.</p>
        <a href="#" class="button">Esto no hace nada</a>
    </div>
</body>
</html>

"""


print(body)

