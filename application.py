import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # For the current username, generate table of the user's stocks
    user_id = session["user_id"]
    portfolio = db.execute("SELECT * FROM portfolio WHERE id = ?", user_id)

    # Get user cash
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    remaining_cash_real = round(user_cash[0]["cash"], 2)
    remaining_cash = usd(remaining_cash_real)

    # Calculate total portfolio value
    total_stock_value = 0
    for stock in portfolio:
        total_stock_value += stock["total"]
        # Convert portfolio values to usd for index display
        stock["price"] = usd(stock["price"])
        stock["total"] = usd(stock["total"])
    portfolio_total_real = round(remaining_cash_real + total_stock_value, 2)
    portfolio_total = usd(portfolio_total_real)

    return render_template("/index.html", portfolio=portfolio, remaining_cash=remaining_cash, portfolio_total=portfolio_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("/buy.html")

    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        # Convert shares to an int, error if failure
        try:
            shares = int(shares)
        except:
            return apology("invalid number of shares", 400)
        if shares < 0 or shares == 0:
            return apology("invalid number of shares", 400)

        # Calculate current purchase size
        result = lookup(symbol)
        print(result)
        # Check if a valid share symbol
        if result == None:
            return apology("stock not found", 400)

        name = result["name"]
        price = result["price"]
        purchase_total = round(price * shares, 2)
        print(f"purchase total: {purchase_total}, for {name} @ {price}")

        # Check if purchase is valid for current user
        user_id = session["user_id"]
        print(f"current user id: {user_id}")
        # Check if there is enough cash
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = user_cash[0]["cash"]
        remaining_cash = round(cash - purchase_total, 2)
        print(f"user cash: {cash}, remaining: {remaining_cash}")

        # If not enough cash, go to apology
        if cash > purchase_total or cash == purchase_total:
            db.execute("INSERT INTO portfolio(id, symbol, name, shares, price, total) VALUES (?, ?, ?, ?, ?, ?)",
                       user_id, symbol, name, int(shares), price, purchase_total)
        else:
            return apology("not enough cash to complete this transaction", 400)

        # Update remaining cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_cash, user_id)

        # Return to index with successful purchase
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        search = request.form.get("symbol")
        # Call lookup to grab the stock data, otherwise send to an error page
        result = lookup(search)
        if result == None:
            return apology("stock not found", 400)
        name = result["name"]
        price = usd(result["price"])
        symbol = result["symbol"]
    # Send search result back to user
        return render_template("/quoted.html", name=name, symbol=symbol, price=price)
    if request.method == "GET":
        return render_template("/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password1 and password2 was submitted
        elif not request.form.get("password"):
            return apology("must provide both passwords", 400)

        elif not request.form.get("confirmation"):
            return apology("must provide both passwords", 400)

        # Add hash value of password to database if both input passwords match, otherwise apologise for mismatched passwords
        password1 = request.form.get("password")
        password2 = request.form.get("confirmation")
        password1_hash = generate_password_hash(request.form.get("password"))
        password2_hash = generate_password_hash(request.form.get("confirmation"))
        compare_p1_hash2 = check_password_hash(password2_hash, password1)
        compare_p2_hash1 = check_password_hash(password1_hash, password2)
        if compare_p1_hash2 == False and compare_p2_hash1 == False:
            return apology("must provide matching passwords", 400)

        # Add username to database if username does not exist
        username = request.form.get("username")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        # Check if username already exists
        for row in rows:
            print(f"{row['username']}")
            if row['username'] == username:
                return apology("invalid username, username already exists", 400)

        # Username / pasword hash can be added to the database
        db.execute("INSERT INTO users(username, hash) VALUES (?, ?)", username, password1_hash)
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Set current user
    user_id = session["user_id"]

    # Return to index with successful purchase
    portfolio = db.execute("SELECT * FROM portfolio WHERE id = ?", user_id)

    # Display list of available stocks
    if request.method == "GET":
        return render_template("/sell.html", portfolio=portfolio)

    # User has selected a stock to sell
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        # Convert shares to an int, error if failure
        try:
            shares = int(shares)
        except:
            return apology("invalid number of shares", 400)
        if shares < 0 or shares == 0:
            return apology("invalid number of shares", 400)

        # Calculate current purchase size
        result = lookup(symbol)
        if result == None:
            return apology("stock not found", 400)
        name = result["name"]
        price = result["price"]
        sell_total = price * shares
        print(f"sell total: {sell_total}, for {name} @ {price}")

        # Check if enough stock is available to sell
        stock_shares = db.execute("SELECT shares FROM portfolio WHERE id = ? and symbol = ?", user_id, symbol)
        max_shares = int(stock_shares[0]["shares"])
        if shares > max_shares:
            return apology("invalid number of shares", 400)
        print(f"stock_shares: {stock_shares}, max: {max_shares}")
        remaining_shares = max_shares - shares
        # Get current cash
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = user_cash[0]["cash"]
        remaining_cash = round(cash + sell_total, 2)
        print(f"cash: {cash}, remaining: {remaining_cash}")

        # Update remaining cash and shares
        db.execute("UPDATE users SET cash = ? WHERE id = ?", remaining_cash, user_id)

        # Remove symbol if no more shares
        if remaining_shares > 0:
            db.execute("UPDATE portfolio SET shares = ? WHERE id = ? and symbol = ?", remaining_shares, user_id, symbol)
        else:
            db.execute("DELETE FROM portfolio WHERE id = ? and symbol = ?", user_id, symbol)

        # Return to index
        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
