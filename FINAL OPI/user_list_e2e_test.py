from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import pytest
import os
import wiremock
import time
from selenium import webdriver
from your_module import app

class TestE2E:

    def setup_method(self):
        with app.test_client() as self.app:
            self.driver = webdriver.Chrome()

    def teardown_method(self):
        self.driver.quit()

    def test_e2e_user_list_endpoint(self):
        with wiremock.Client() as mock_server:
            mock_server.stub_for(wiremock.get(wiremock.url_path_matching('/api/users/lastSeen.*'))
                                  .willReturn(wiremock.a_response()
                                  .with_status(200)
                                  .with_body_json({
                                      'data': [
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
                                  })))

            base_url = os.environ.get('BASE_URL')
            url = f'{base_url}/api/users/list'
            self.driver.get(url)

            user_data_table = WebDriverWait(self.driver, 10).until(
                EC.visibility_of_element_located((By.ID, 'user-data-table'))
            )

            rows = user_data_table.find_elements(By.TAG_NAME, 'tr')
            expected_data = [
                ['userId', 'isOnline', 'lastSeenDate'],
                ['user1', True, '2023-01-01T12:00:00'],
                ['user2', False, '2023-01-01T13:00:00']
            ]
            actual_data = [[cell.text for cell in row.find_elements(By.TAG_NAME, 'td')] for row in rows]
            assert actual_data == expected_data

if __name__ == '__main__':
    pytest.main()
