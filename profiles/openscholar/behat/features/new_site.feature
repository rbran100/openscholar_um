Feature: Testing the creation of the a new site.

  @api
  Scenario: Test the creation of a new site and verify that we don't get JS alert.
    Given I am logged in as "admin"
    When I visit "/"
     And I click "Create your site"
     And I fill "edit-domain" with random text
     And I press "edit-submit"
     And I visit the site "random"
    Then I should see "Your site's front page is set to display your bio by default."
