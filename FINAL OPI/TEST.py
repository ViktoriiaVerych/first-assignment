import pytest
from unittest.mock import Mock, patch
import json
from datetime import datetime
from your_module import get_data, update_data, fand_update_data, load_data, get_users_online

class TestYourCode:

    def setup_method(self):
        # Create a sample data file with initial data
        initial_data = [
            {
                'userId': 'user1',
                'isOnline': True,
                'lastSeenDate': '2023-01-01T12:00:00'
            },
            {
                'userId': 'user2',
                'isOnline': False,
                'lastSeenDate': '2023-01-01T13:00:00'
            }
        ]
        with patch('builtins.open', create=True) as mock_open:
            mock_file = mock_open.return_value
            mock_file.__enter__.return_value = mock_file
            mock_file.read.return_value = json.dumps(initial_data)

            # Call the code under test
            self.data = load_data()

    @pytest.mark.parametrize("offset, expected_data", [
        (0, []),
        (1, []),
        (10, []),
    ])
    def test_get_data(self, offset, expected_data):
        # Mock the requests.get method
        with patch('your_module.requests.get') as mock_get:
            # Mock a successful response
            mock_get.return_value.status_code = 200
            mock_get.return_value.json.return_value = {'data': []}

            data = get_data(offset)
            assert data == expected_data
            mock_get.assert_called_with('https://sef.podkolzin.consulting/api/users/lastSeen?offset={}'.format(offset))

            # Mock a request error
            mock_get.side_effect = Exception('Error')
            data = get_data(offset)
            assert data == expected_data

    def test_update_data(self):
        user = {
            'userId': 'user1',
            'isOnline': False,
            'lastSeenDate': '2023-01-01T14:00:00'
        }
        previous_state = {'user1': {'userId': 'user1', 'isOnline': True, 'lastSeenDate': '2023-01-01T12:00:00', 'onlinePeriods': []}}
        updated_user = update_data(user, previous_state)
        assert updated_user['isOnline'] == False
        assert len(updated_user['onlinePeriods']) == 1

    def test_load_data(self):
        data = load_data()
        assert isinstance(data, list)

    def test_get_users_online(self):
        test_date = datetime.strptime('2023-01-01T13:30:00', '%Y-%m-%dT%H:%M:%S')
        users_online = get_users_online(test_date)
        assert users_online == 1

    def test_fand_update_data(self):
        # Mock the get_data function
        with patch('your_module.get_data') as mock_get_data, \
                patch('builtins.open', create=True) as mock_open:
            mock_file = mock_open.return_value
            mock_file.__enter__.return_value = mock_file
            mock_file.read.return_value = json.dumps([
                {
                    'userId': 'user1',
                    'isOnline': True,
                    'lastSeenDate': '2023-01-01T12:00:00'
                },
                {
                    'userId': 'user2',
                    'isOnline': False,
                    'lastSeenDate': '2023-01-01T13:00:00'
                }
            ])

            mock_get_data.return_value = [
                {
                    'userId': 'user3',
                    'isOnline': True,
                    'lastSeenDate': '2023-01-01T15:00:00'
                }
            ]
            fand_update_data()

            # Verify the updated data
            mock_file.write.assert_called_once_with(json.dumps([
                {
                    'userId': 'user1',
                    'isOnline': True,
                    'lastSeenDate': '2023-01-01T12:00:00'
                },
                {
                    'userId': 'user2',
                    'isOnline': False,
                    'lastSeenDate': '2023-01-01T13:00:00'
                },
                {
                    'userId': 'user3',
                    'isOnline': True,
                    'lastSeenDate': '2023-01-01T15:00:00'
                }
            ]))


if __name__ == '__main__':
    pytest.main()
