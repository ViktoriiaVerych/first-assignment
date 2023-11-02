from marshmallow import Schema, fields, validate
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.sql import text
from apscheduler.schedulers.background import BackgroundScheduler
from flask import Blueprint, Flask, request, jsonify

import data_processing  # Assuming you have a module named data_processing

app = Flask(__name)

scheduler = BackgroundScheduler()
scheduler.add_job(data_processing.fetch_and_update_data, 'interval', minutes=60)
scheduler.start()

class ForgetUserSchema(Schema):
    userId = fields.Int(required=True, validate=validate.Range(min=1))

@app.route('/api/user/forget', methods=['POST'])
def forget_user():
    request_body = request.get_json()
    
    try:
        data = ForgetUserSchema().load(request_body)
    except ValidationError as e:
        return jsonify({'error': str(e)}), 400
    
    userId = data['userId']
    
    if data_processing.check_user_exists(userId):
        db = SQLAlchemy()
        conn = db.engine.connect()
        stmt = text("DELETE FROM users WHERE id = :id")
        conn.execute(stmt, id=userId)
        return jsonify({'userId': userId})
    else:
        return jsonify({'error': 'User does not exist'}), 404

def handle_invalid_user(error):
    return jsonify({'error': 'Invalid userId'}), 404

def get_online_time(userId):
    user_data = data_processing.update_user_data(userId)
    
    if user_data is None:
        return handle_invalid_user(404)

    online_time = data_processing.calculate_online_time(user_data)
    return jsonify({'userId': userId, 'onlineTime': online_time})

def get_average_times(userId):
    user_data = data_processing.update_user_data(userId)
    
    if user_data is None:
        return handle_invalid_user(404)

    weekly_avg, daily_avg = data_processing.calculate_average_times(user_data)
    return jsonify({'userId': userId, 'weeklyAverage': weekly_avg, 'dailyAverage': daily_avg})

def forget_user():
    request_body = request.get_json()
    userId = request_body.get('userId')
    
    if data_processing.check_user_exists(userId):
        data_processing.delete_user_data(userId)
        return jsonify({'userId': userId})
    else:
        return jsonify({'error': 'User does not exist'}), 404

@app.route('/api/stats/user/online_time/<int:userId>', methods=['GET'])
def get_online_time_route(userId):
    return get_online_time(userId)

@app.route('/api/stats/user/average/<int:userId>', methods=['GET'])
def get_average_times_route(userId):
    return get_average_times(userId)

@app.route('/api/user/forget', methods=['POST'])
def forget_user_route():
    return forget_user()

@app.errorhandler(404)
def handle_invalid_user(error):
    return jsonify({'error': 'Invalid userId'}), 404

@app.errorhandler(500)
def handle_server_error(error):
    return jsonify({'error': 'Internal server error'}), 500

stats_routes = Blueprint('stats_routes', __name__)

@stats_routes.route('/api/stats/user/online_time/<int:userId>', methods=['GET'])
def get_online_time(userId):
    try:
        user_data = data_processing.update_user_data(userId)

        if user_data is None:
            return handle_invalid_user(404)

        online_time = data_processing.calculate_online_time(user_data)
        return jsonify({'userId': userId, 'onlineTime': online_time})
    except Exception as e:
        return handle_server_error(500)

@stats_routes.route('/api/stats/user/average/<int:userId>', methods=['GET'])
def get_average_times(userId):
    try:
        user_data = data_processing.update_user_data(userId)

        if user_data is None:
            return handle_invalid_user(404)

        weekly_avg, daily_avg = data_processing.calculate_average_times(user_data)
        return jsonify({'userId': userId, 'weeklyAverage': weekly_avg, 'dailyAverage': daily_avg})
    except Exception as e:
        return handle_server_error(500)

user_routes = Blueprint('user_routes', __name__)

@user_routes.route('/api/user/forget', methods=['POST'])
def forget_user():
    try:
        request_body = request.get_json()
        userId = request_body.get('userId')

        if data_processing.check_user_exists(userId):
            data_processing.delete_user_data(userId)
            return jsonify({'userId': userId})
        else:
            return jsonify({'error': 'User does not exist'}), 404
    except Exception as e:
        return handle_server_error(500)

app.register_blueprint(stats_routes)
app.register_blueprint(user_routes)

if __name__ == "__main__":
    app.run()
